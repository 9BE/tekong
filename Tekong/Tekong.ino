#include "Arduino.h"
#include <LocSpiff.h>
#include <LocWiFi.h>

FileInfo_t 	ssid;
LocWiFi		*locWiFi;
int xValWiFi = 0;

void setupSSID();

//===============================================================================
void setup()
{
	Serial.begin(912600, SERIAL_8N1, 3, 1);
	delay(1000);
	log_i("\n\n\n\nSalam Dunia dari %s\n\n\n\n", __FILE__);


	setupSSID();

	locWiFi = new LocWiFi(0,3000, &xValWiFi);
	xValWiFi = lw_wifi_sta;
}

//===============================================================================
void loop()
{
	delay(1000);


}

//===============================================================================
inline void setupSSID() {
	LocSpiff 	*locSpiff;

	locSpiff = new LocSpiff;
	ssid = locSpiff->getInfo("/ssid.txt");
	if(ssid.filename == "/ssid.txt"){
		log_i("Exist");
	}
	else{
		log_i("Not Exist - Creating default credentials");
		locSpiff->appendFile("/ssid.txt", "sta,ideapad,sawabatik1\n");
		locSpiff->appendFile("/ssid.txt", "sta,AndroidAP,sawabatik\n");
		locSpiff->appendFile("/ssid.txt", "sta,GF_Wifi_2.4GHz,Gr33nF1nd3r2018\n");
		locSpiff->appendFile("/ssid.txt", "ap,NiNe,AsamBoiqqq\n");
	}
	delete locSpiff;
}



