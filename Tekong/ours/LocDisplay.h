/*
 * LocDisplay.h
 *
 *  Created on: Oct 20, 2019
 *      Author: annuar
 */

#ifndef OURS_LOCDISPLAY_H_
#define OURS_LOCDISPLAY_H_

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "Adafruit_ILI9341.h"



#include "SSD1306Spi.h"
#include "OLEDDisplayUi.h"
#include <SPI.h>

class LocDisplay {
public:
	LocDisplay();
	virtual ~LocDisplay();
};

#endif /* OURS_LOCDISPLAY_H_ */
