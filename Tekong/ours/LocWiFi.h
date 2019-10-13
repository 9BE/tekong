/*
 * LocWiFi.h
 *
 *  Created on: Oct 13, 2019
 *      Author: annuar
 */

#ifndef OURS_LOCWIFI_H_
#define OURS_LOCWIFI_H_

#include "Arduino.h"

class LocWiFi {
public:
	LocWiFi(int core, int loopDelay, int *lookVal);
	virtual ~LocWiFi();
	static void loop(void * parameter);
};

#endif /* OURS_LOCWIFI_H_ */
