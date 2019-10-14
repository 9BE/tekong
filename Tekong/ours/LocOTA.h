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

class LocOTA {
private:
	int 	_loopDelay;
	char	*_site;
	HTTPClient _http;
public:
	LocOTA(int core, int loopDelay, char *site);
	static void loop(void * parameter);
};

#endif /* OURS_LOCOTA_H_ */
