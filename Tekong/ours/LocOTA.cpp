/*
 * LocOTA.cpp
 *
 *  Created on: Oct 14, 2019
 *      Author: annuar
 */

#include 	<LocOTA.h>
#include	"ESP32httpUpdate.h"
#include 	"SPIFFS.h"
#include 	"FS.h"

//#include	"HTTPClient.h"

LocOTA	*iniOTA;
TaskHandle_t loopLocOTA= NULL;



LocOTA::LocOTA(int core, int loopDelay, String site) {
	//aza
	iniOTA = this;


	// loopDelay digunakan untuk check server
	iniOTA->_loopDelay = loopDelay;
	iniOTA->_site = site;

	Serial.println(iniOTA->_site);
	log_i("Apakah %s", iniOTA->_site.c_str());

	iniOTA->_url = "http://nine-server.000webhostapp.com/nbe/masa.php?file=";
	iniOTA->_url += iniOTA->_site;
	log_i("Jadi %s", iniOTA->_url.c_str());



	xTaskCreatePinnedToCore(iniOTA->loop, "loopLocOTA", 3072, NULL, 1, &loopLocOTA, core);

}


void LocOTA::loop(void* parameter) {
	//aza
	String payload = "";
	LocSpiff *locSpiff;
	int scan = 0;
	bool scanEn = false;

	int httpCode;

	locSpiff = new LocSpiff;
	iniOTA->_latestFileTimeStamp = locSpiff->readFile("/timestamp.txt");
//	free(locSpiff);

	iniOTA->_latestFileTimeStamp.trim();

	log_i("Timestamp = %s", iniOTA->_latestFileTimeStamp.c_str());
	esp_task_wdt_reset();

	while(true){

		if(scanEn){
			if((WiFi.getMode() == WIFI_MODE_STA) || (WiFi.getMode() == WIFI_MODE_APSTA)){

				log_i("WiFi Mode = %d, site=%s", WiFi.getMode(), iniOTA->_site.c_str());
				esp_task_wdt_reset();


				iniOTA->_http.begin("http://nine-server.000webhostapp.com/nbe/masa.php?file=" + iniOTA->_site);
				httpCode = iniOTA->_http.GET();
				esp_task_wdt_reset();

				if(httpCode ==  200){
					scanEn = false;
					scan = 0;
					payload = iniOTA->_http.getString();
					payload.trim();
					iniOTA->_http.end();

					if(payload.equals(iniOTA->_latestFileTimeStamp)){
						log_i("SAMA");
					}
					else{
						log_i("TAK SAMA");

						esp_task_wdt_reset();
						ESPhttpUpdate.rebootOnUpdate(false);
						t_httpUpdate_return ret = ESPhttpUpdate.update("http://nine-server.000webhostapp.com/nbe/" + iniOTA->_site);

	//					Serial.print("return = ");
	//					Serial.println(ret);
						if(ret == HTTP_UPDATE_OK){
							iniOTA->_http.end();
							esp_task_wdt_reset();
							delay(1000);
	//						LocSpiff *ddd;
	//						ddd = new LocSpiff;
							bool wrt=false;

							while(wrt == false){
								wrt = locSpiff->writeFile("/timestamp.txt", payload.c_str());
								delay(10);
							}
							log_i("=====================DONE=============================== %s", payload.c_str());
							delay(2000);
							ESP.restart();

						}
					}
				}
				else{
					log_i("Connection error? %d", httpCode);
					payload = "error";
				}
				iniOTA->_http.end();
			}
			else{
				log_w("NO OTA - No WiFi connection");
			}
		}

		esp_task_wdt_reset();


		delay(1000);
		scan += 1000;
		if(scan > iniOTA->_loopDelay){
			scan = 0;
			scanEn = true;
		}
	}
}
