#include "Arduino.h"
#include <LocSpiff.h>
#include <LocWiFi.h>




FileInfo_t ssid;

LocSpiff 	*locSpiff;
LocWiFi		*locWiFi;

int xValWiFi = 0;

void setupSSID();


//===============================================================================
void setup()
{
	Serial.begin(912600, SERIAL_8N1, 3, 1);
	delay(1000);
	log_i("\n\n\n\nSalam Dunia dari %s\n\n\n\n", __FILE__);

	locSpiff = new LocSpiff;
	setupSSID();

	locWiFi = new LocWiFi(0,3000, &xValWiFi);
	xValWiFi = lw_wifi_apsta;
//	xValWiFi = lw_wifi_ap;


}

//===============================================================================
void loop()
{
	delay(1000);
//	log_i("Salam dunia");
//	xValWiFi ++;
//	xValWiFi = lw_wifi_apsta;


}

//===============================================================================
inline void setupSSID() {
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
}


//	locSpiff->listAllFiles();
//	locSpiff->listDir("/", 3);
//	log_i("name = %s", fi.filename.c_str());
//	log_i("Size = %d", fi.size);
//
//
//	fi = locSpiff->getInfo("/logo.jpg");
//	log_i("Logo = %s", fi.filename.c_str());
//	log_i("Size = %d", fi.size);

