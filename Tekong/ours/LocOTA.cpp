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
	iniOTA->_loopDelay = loopDelay;	// loopDelay digunakan untuk check server
	iniOTA->_site = site;
	xTaskCreatePinnedToCore(iniOTA->loop, "loopLocOTA", 3072, NULL, 1, &loopLocOTA, core);
}

void LocOTA::loop(void* parameter) {
	//aza
	String payload = "";
	LocSpiff *locSpiff;
	int scan = 0;
	bool scanEn = false;

	int httpCode;

	locSpiff = new LocSpiff("LocOTA::loop");

	iniOTA->_latestFileTimeStamp = locSpiff->readFile("/timestamp.txt");

	if(iniOTA->_latestFileTimeStamp.length() < 5){
		iniOTA->_latestFileTimeStamp = "NO TIME STAMP";
		locSpiff->writeFile("/timestamp.txt", iniOTA->_latestFileTimeStamp.c_str());
	}


	log_i("SINI");

	delete locSpiff;

	iniOTA->_latestFileTimeStamp.trim();

	log_i("Timestamp = %s", iniOTA->_latestFileTimeStamp.c_str());
	esp_task_wdt_reset();

	while(true){

		if(scanEn){
			if((WiFi.getMode() == WIFI_MODE_STA) || (WiFi.getMode() == WIFI_MODE_APSTA)){
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

					log_i("Payload = %s; FileTimeStamp = %s", payload.c_str(), iniOTA->_latestFileTimeStamp.c_str());

					if(payload.equals(iniOTA->_latestFileTimeStamp)){
						log_i("SAMA");
					}
					else{
						log_i("TAK SAMA");

						esp_task_wdt_reset();
						ESPhttpUpdate.rebootOnUpdate(false);
						t_httpUpdate_return ret = ESPhttpUpdate.update("http://nine-server.000webhostapp.com/nbe/" + iniOTA->_site);

						if(ret == HTTP_UPDATE_OK){
							iniOTA->_http.end();
							esp_task_wdt_reset();

							LocSpiff *ddd;
							ddd = new LocSpiff("HTTP_UPDATE_OK");
							bool wrt=false;

							ddd->deleteFile("/timestamp.txt");

							while(wrt == false){
								wrt = ddd->writeFile("/timestamp.txt", payload.c_str());
								delay(10);
							}
							log_i("=====================DONE=============================== %s", payload.c_str());

							ddd->listAllFiles();
							delete ddd;
							delay(2000);
							ESP.restart();

						}
						if(ret == HTTP_UPDATE_FAILED){
							scanEn = true;
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
