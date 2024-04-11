//Arduino Mega Wifi: https://www.youtube.com/watch?v=2Hgm8VkpUlw https://www.instructables.com/Arduino-MEGA-2560-With-WiFi-Built-in-ESP8266/
//Pin references: https://youtu.be/HowngNXaVGg?si=awLtGCaupARWllWt&t=421

#define BLYNK_TEMPLATE_ID "TMPL6wjoazYhu"
#define BLYNK_TEMPLATE_NAME "Mega 2560 IDP"
#define BLYNK_AUTH_TOKEN "fQuPJ3sYWl10H1vJAV-EHSngVjZUMLK4"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Boss_Digi";  // type your wifi name
char pass[] = "1011372888";  // type your wifi password

float temperature_1, temperature_2, temperature_3, temperature_4, temperature_5, temperature_6;
float pressure1, pressure2, pressure3, pressure4, pressure5, pressure6;
float flow;

BlynkTimer timer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(100L, sendSensor);
}

String splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    String msg = "";
    while(Serial.available()){
      msg += char(Serial.read());
    }
    temperature_1 = splitString(msg, ';', 0).toFloat();
    temperature_2 = splitString(msg, ';', 1).toFloat();
    temperature_3 = splitString(msg, ';', 2).toFloat();
    temperature_4 = splitString(msg, ';', 3).toFloat();
    temperature_5 = splitString(msg, ';', 4).toFloat();
    temperature_6 = splitString(msg, ';', 5).toFloat();
    pressure1 = splitString(msg, ';', 6).toFloat();
    pressure2 = splitString(msg, ';', 7).toFloat();
    pressure3 = splitString(msg, ';', 8).toFloat();
    pressure4 = splitString(msg, ';', 9).toFloat();
    pressure5 = splitString(msg, ';', 10).toFloat();
    pressure6 = splitString(msg, ';', 11).toFloat();
    flow = splitString(msg, ';', 12).toFloat();
    sendSensor();
    Serial.print(msg);
  }
  Blynk.run();
  timer.run();
}

void sendSensor(){

  //Blynk.virtualWrite(V0, thermistorResistance_1);
  Blynk.virtualWrite(V0, temperature_1);
  Blynk.virtualWrite(V1, temperature_2);
  Blynk.virtualWrite(V2, temperature_3);
  Blynk.virtualWrite(V3, temperature_4);
  Blynk.virtualWrite(V4, temperature_5);
  Blynk.virtualWrite(V5, temperature_6);
  Blynk.virtualWrite(V6, pressure1);
  Blynk.virtualWrite(V7, pressure2);
  Blynk.virtualWrite(V8, pressure3);
  Blynk.virtualWrite(V9, pressure4);
  Blynk.virtualWrite(V10, pressure5);
  Blynk.virtualWrite(V11, pressure6);
  Blynk.virtualWrite(V12, flow);
}
  
