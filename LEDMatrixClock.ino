#include <SPI.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

WiFiUDP ntpUDP;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 1*3600, 60000);


MD_Parola P = MD_Parola(MD_MAX72XX::FC16_HW, D2, 4);

uint8_t scrollSpeed = 40;    // default frame delay value
textEffect_t scrollEffect = PA_SCROLL_LEFT;
//textEffect_t scrollEffect = PA_PRINT;
textPosition_t scrollAlign = PA_CENTER;
uint16_t scrollPause = 0; // in milliseconds

WiFiManager wifiManager;

const unsigned long targetTime = 1670198400;
unsigned long lastDiff = 0;

#define BUF_SIZE 11

char buffer[BUF_SIZE] = "";
    
void setup() {
  // Seems to make flashing more reliable
  delay(100);

  P.begin();

  Serial.begin(115200);
  Serial.println();
  Serial.print("MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.println();
  Serial.println("Starting ...");
  P.setIntensity(1);
  P.displayText("...", PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
  P.displayAnimate();

  wifiManager.autoConnect("TimerSetup", "timertimer");
  timeClient.begin();

  P.displayText(buffer, scrollAlign, scrollSpeed, scrollPause, scrollEffect, scrollEffect);

  Serial.println("Done");
}

void loop() {
  timeClient.update();

  unsigned long timeDiff = (targetTime - timeClient.getEpochTime());

  if(P.displayAnimate()) {
    if(timeDiff != lastDiff) {
      sprintf(buffer, "%lu", timeDiff);
      Serial.println(buffer);
      lastDiff = timeDiff;
    }
    P.displayReset();
  }
  
}
