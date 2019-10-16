/*
 * LocOTA.h
 *
 *  Created on: Oct 14, 2019
 *      Author: annuar
 */

#ifndef OURS_LOCOTA_H_
#define OURS_LOCOTA_H_

#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <LocSpiff.h>

#include	"ESP32httpUpdate.h"

class LocOTA {
private:
	int 	_loopDelay;
	String 	_site;
	HTTPClient _http;
	String _latestFileTimeStamp = "";
	String _url;
public:
	LocOTA(int core, int loopDelay, String site);
	static void loop(void * parameter);
};

#endif /* OURS_LOCOTA_H_ */
