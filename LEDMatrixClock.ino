#include <SPI.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

WiFiUDP ntpUDP;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "fritz.box", 1*3600, 60000);


MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, D2, 4);

uint8_t scrollSpeed = 25;    // default frame delay value
//textEffect_t scrollEffect = PA_SCROLL_LEFT;
textEffect_t scrollEffect = PA_PRINT;
textPosition_t scrollAlign = PA_CENTER;
uint16_t scrollPause = 2000; // in milliseconds
#define BUF_SIZE 9

WiFiManager wifiManager;

void setup() {
  // Seems to make flashing more reliable
  delay(100);

  P.begin();

  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting ...");
  P.setIntensity(1);
  P.displayText("...", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();

  wifiManager.autoConnect("TimerSetup", "timertimer");
  timeClient.begin();

  P.displayAnimate();

  ArduinoOTA.setHostname("ledclock");
  ArduinoOTA.begin();
  Serial.println("Done");
}

void loop() {
  timeClient.update();
  ArduinoOTA.handle();

  int hours = timeClient.getHours();
  int minutes = timeClient.getMinutes();
  int seconds = timeClient.getSeconds();

  char buffer[BUF_SIZE];

  sprintf(buffer, "%02d:%02d", hours, minutes, seconds);

  P.displayText(buffer, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);
  if(P.displayAnimate()) {
    P.displayReset();
  }
  
}
