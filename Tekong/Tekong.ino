#include "Arduino.h"
#include <LocSpiff.h>
#include <LocWiFi.h>
#include <LocOTA.h>
#include <Update.h>
#include <LocMQTT.h>
#include <LocDisplay.h>

FileInfo_t 	ssid;
LocWiFi		*locWiFi;
LocOTA		*locOTA;
LocMQTT		*locMqtt;
LocDisplay	*locDisplay;
int xValWiFi = 0;

// for MQTT
time_t tickNyamukTime = 0;
void TickNyamuk();
bool tockBeat;
bool mqttEnabled=true;

#define pinBuzzer 5


void setupSPIFFiles();

//===============================================================================
void setup()
{
	String binFile;

	pinMode(pinBuzzer, OUTPUT);
	digitalWrite(pinBuzzer, LOW);

	Serial.begin(912600, SERIAL_8N1, 3, 1);
	delay(1000);
	log_i("\n\n\n\nSalam Dunia dari %s\n\n\n\n", __FILE__);
	log_i("Memory = %d", String(esp_get_free_heap_size()).c_str());




	binFile = __FILE__;
	binFile = binFile.substring(3, binFile.length());
	binFile = binFile.substring(0, binFile.length()-3);
	binFile += "bin";

	setupSPIFFiles();

	locOTA = new LocOTA(0, 120000, binFile);
	delay(1000);

	locWiFi = new LocWiFi(0,3000, &xValWiFi);
//	xValWiFi = lw_wifi_apsta;
	xValWiFi = lw_wifi_sta;

	delay(1000);

	locMqtt = new LocMQTT;
	locDisplay = new LocDisplay(1, 1000);


}

//===============================================================================
void loop()
{

	delay(1000);

	if(Update.isRunning()){
		mqttEnabled = false;
		Serial.print(Update.progress());
		Serial.print(" @ ");
		Serial.println(Update.size());

	}
	else{
		mqttEnabled = true;
	}

	TickNyamuk();

}



//-------------------------------------------------------------------------------
void Sengat(String topic, String message) {
	if(mqttEnabled){
		locMqtt->hantar(topic, message);
		log_i("---------------------------------> %s", message.c_str());
	}
}

//-------------------------------------------------------------------------------
inline void TickNyamuk() {
	locMqtt->update();
	if((millis()-tickNyamukTime) > 60000){
		tickNyamukTime = millis();
		tockBeat = !tockBeat;
		Sengat("ayamhutan/beat", tockBeat?"1":"0");
	}
}

//-------------------------------------------------------------------------------
inline void setupSPIFFiles() {
	//aza
	LocSpiff 	*locSpiff;
	FileInfo_t	info;

	locSpiff = new LocSpiff("setupSPIFFiles");

	locSpiff->listAllFiles();

//	locSpiff->format(); 	// ! ! ! ! ! ! ! ! ! Danger ! ! ! ! ! ! ! ! ! !

	//Setup latestVersion time stamp--------------------------------------------
//	"/timestamp.txt"
	info = locSpiff->getInfo("/timestamp.txt");
	if(info.filename == "/timestamp.txt"){
		log_i("info /timestamp.txt = EXIST");
	}
	else{



	}

	//Setup SSID----------------------------------------------------------------
	ssid = locSpiff->getInfo("/ssid.txt");
	if(ssid.filename == "/ssid.txt"){
		log_i("info /ssid.txt = EXIST");
		log_i("Exist");
	}
	else{
		log_i("Not Exist - Creating default credentials");
		locSpiff->deleteFile("/ssid.txt");
		locSpiff->appendFile("/ssid.txt", "sta,ideapad,sawabatik1\n");
		locSpiff->appendFile("/ssid.txt", "sta,AndroidAP,sawabatik\n");
		locSpiff->appendFile("/ssid.txt", "sta,GF_Wifi_2.4GHz,Gr33nF1nd3r2018\n");
		locSpiff->appendFile("/ssid.txt", "ap,NiNe,AsamBoiqqq\n");
	}



	delete locSpiff;
}
