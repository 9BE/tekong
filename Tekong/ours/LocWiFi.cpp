/*
 * LocWiFi.cpp
 *
 *  Created on: Oct 13, 2019
 *      Author: annuar
 */

#include <LocWiFi.h>

LocWiFi	*aaa;
TaskHandle_t loopLocWiFi= NULL;
int _loopDelay;
int * _lookVal;

LocWiFi::LocWiFi(int core, int loopDelay, int *lookVal) {
	aaa = this;
	_loopDelay = loopDelay;
	_lookVal = lookVal;

	xTaskCreatePinnedToCore(aaa->loop, "loopLocWiFi", 3072, NULL, 1, &loopLocWiFi, core);

}

LocWiFi::~LocWiFi() {


}

void LocWiFi::loop(void* parameter) {
	//init


	while(true){

		log_i("LocWiFi::loop %d", *_lookVal);



		delay(_loopDelay);
	}
}
