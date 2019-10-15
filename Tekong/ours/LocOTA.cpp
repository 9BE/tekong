/*
 * LocOTA.cpp
 *
 *  Created on: Oct 14, 2019
 *      Author: annuar
 */

#include <LocOTA.h>
#include	"ESP32httpUpdate.h"

//#include	"HTTPClient.h"

LocOTA	*iniOTA;
TaskHandle_t loopLocOTA= NULL;



LocOTA::LocOTA(int core, int loopDelay, char *site) {
	//aza
	iniOTA = this;
	iniOTA->_loopDelay = loopDelay;
	iniOTA->_site = site;

	xTaskCreatePinnedToCore(iniOTA->loop, "loopLocOTA", 3072, NULL, 1, &loopLocOTA, core);

}


void LocOTA::loop(void* parameter) {
	//aza
	String payload = "";
	LocSpiff *locSpiff;

	locSpiff = new LocSpiff;
	iniOTA->_latestFileTimeStamp = locSpiff->readFile("/timestamp.txt");
//	free(locSpiff);


//	iniOTA->_latestFileTimeStamp = "1571039415";
	iniOTA->_latestFileTimeStamp.trim();

	log_i("Timestamp = %s", iniOTA->_latestFileTimeStamp.c_str());

	while(true){
		if((WiFi.getMode() == WIFI_MODE_STA) || (WiFi.getMode() == WIFI_MODE_APSTA)){
			log_i("WiFi Mode = %d, site=%s", WiFi.getMode(), iniOTA->_site);
			iniOTA->_http.begin("http://nine-server.000webhostapp.com/nbe/masa.php?file=" + String(iniOTA->_site));
			int httpCode = iniOTA->_http.GET();
			if(httpCode ==  200){
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
					t_httpUpdate_return ret = ESPhttpUpdate.update("http://nine-server.000webhostapp.com/nbe/" + String(iniOTA->_site));

//					Serial.print("return = ");
//					Serial.println(ret);
					if(ret == HTTP_UPDATE_OK){
						iniOTA->_http.end();
						delay(1000);
//						LocSpiff *ddd;
//						ddd = new LocSpiff;
						locSpiff->writeFile("/timestamp.txt", payload.c_str());
//						free(ddd);
						log_i("=====================DONE=============================== %s", payload.c_str());
						delay(2000);
						ESP.restart();
					}
				}
			}
			else{
				payload = "error";
			}

			iniOTA->_http.end();

//			Serial.print(payload);
		}
		else{
			log_w("NO OTA - No WiFi connection");
		}
		esp_task_wdt_reset();
		delay(iniOTA->_loopDelay);
	}
}
