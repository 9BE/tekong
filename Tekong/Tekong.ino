#include "Arduino.h"
#include <LocSpiff.h>
#include <LocWiFi.h>
#include <LocOTA.h>
#include <Update.h>
#include <LocMQTT.h>

FileInfo_t 	ssid;
LocWiFi		*locWiFi;
LocOTA		*locOTA;
LocMQTT		*locMqtt;
int xValWiFi = 0;

// for MQTT
time_t tickNyamukTime = 0;
void TickNyamuk();
bool tockBeat;
bool mqttEnabled=true;


void setupSSID();

//===============================================================================
void setup()
{
	String binFile;
	Serial.begin(912600, SERIAL_8N1, 3, 1);
	delay(1000);
	log_i("\n\n\n\nSalam Dunia dari %s\n\n\n\n", __FILE__);
	log_i("Memory = %d", String(esp_get_free_heap_size()).c_str());

	binFile = __FILE__;
	binFile = binFile.substring(3, binFile.length());
	binFile = binFile.substring(0, binFile.length()-3);
	binFile += "bin";

	setupSSID();

	locOTA = new LocOTA(0, 120000, binFile);
	delay(1000);

	locWiFi = new LocWiFi(0,3000, &xValWiFi);
//	xValWiFi = lw_wifi_apsta;
	xValWiFi = lw_wifi_sta;

	delay(10000);

	locMqtt = new LocMQTT;
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

//===============================================================================
inline void setupSSID() {
	//aza
	LocSpiff 	*locSpiff;

	locSpiff = new LocSpiff("setupSSID");

	ssid = locSpiff->getInfo("/ssid.txt");
	if(ssid.filename == "/ssid.txt"){
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

//===============================================================================
void Sengat(String topic, String message) {
	if(mqttEnabled){
		locMqtt->hantar(topic, message);
		log_i("---------------------------------> %s", message.c_str());
	}
}

inline void TickNyamuk() {

	locMqtt->update();
	if((millis()-tickNyamukTime) > 10000){
		log_i("TickNyamuk------------------------------------------------------------------>");
		tickNyamukTime = millis();
		tockBeat = !tockBeat;
		Sengat("ayamhutan/beat", tockBeat?"1":"0");
	}


}


