/*
 * LocOTA.cpp
 *
 *  Created on: Oct 14, 2019
 *      Author: annuar
 */

#include <LocOTA.h>


//#include	"HTTPClient.h"

LocOTA	*iniOTA;
TaskHandle_t loopLocOTA= NULL;



LocOTA::LocOTA(int core, int loopDelay, char *site) {
	iniOTA = this;
	iniOTA->_loopDelay = loopDelay;
	iniOTA->_site = site;

	xTaskCreatePinnedToCore(iniOTA->loop, "loopLocOTA", 3072, NULL, 1, &loopLocOTA, core);

}


void LocOTA::loop(void* parameter) {
	String payload = "";

	while(true){
		if((WiFi.getMode() == WIFI_MODE_STA) || (WiFi.getMode() == WIFI_MODE_APSTA)){
			log_i("WiFi Mode = %d, site=%s", WiFi.getMode(), iniOTA->_site);
			iniOTA->_http.begin("http://nine-server.000webhostapp.com/");
			int httpCode = iniOTA->_http.GET();
			if(httpCode ==  200){
				payload = iniOTA->_http.getString();
			}
			else{
				payload = "yyy?";
			}

			iniOTA->_http.end();

			Serial.print(payload);
		}
		else{
			log_w("NO OTA");
		}
		delay(iniOTA->_loopDelay);
	}
}
