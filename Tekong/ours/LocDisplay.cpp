/*
 * LocDisplay.cpp
 *
 *  Created on: Oct 20, 2019
 *      Author: annuar
 */

#include <LocDisplay.h>

LocDisplay	*iniDisplay;
TaskHandle_t loopLocDisplay = NULL;

LocDisplay::LocDisplay(int core, int loopDelay) {

	iniDisplay = this;
	iniDisplay->_loopDelay = loopDelay;

	xTaskCreatePinnedToCore(iniDisplay->loop, "loopLocOTA", 3072, NULL, 1, &loopLocDisplay, core);


}

LocDisplay::~LocDisplay() {
	// TODO Auto-generated destructor stub
}



void LocDisplay::loop(void* param) {
	pinMode(2, OUTPUT);

	while(true){



		digitalWrite(2, HIGH);
		delay(iniDisplay->_loopDelay);
		digitalWrite(2, LOW);
		delay(iniDisplay->_loopDelay);
	}
}
