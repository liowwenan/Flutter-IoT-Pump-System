//Source: https://randomnerdtutorials.com/esp32-data-logging-firebase-realtime-database/
//https://www.youtube.com/watch?v=aO92B-K4TnQ https://www.youtube.com/watch?v=4vbXwtqCPOc&list=PLEDaotIsUUtiPeA21hq7B9FgO_MV-5a4H&index=2

#include <Firebase_ESP_Client.h>
#include <WiFi.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

#define WIFI_SSID "Boss_Digi"  // type your wifi name
#define WIFI_PASSWORD "1011372888"  // type your wifi password

// Insert Firebase project API Key
#define API_KEY "AIzaSyBedM-GzU82_7fV8gEMww3iMC0FLe1oAaY"

// Insert Authorized Email and Corresponding Password
//#define USER_EMAIL "REPLACE_WITH_THE_USER_EMAIL"
//#define USER_PASSWORD "REPLACE_WITH_THE_USER_PASSWORD" //Removed user email & password because Firebase we set the user authorisation as anynomous

// Insert RTDB URLefine the RTDB URL
#define DATABASE_URL "https://iot-pump-system-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Define Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Variable to save USER UID
String uid;

// Database main path (to be updated in setup with the user UID)
String databasePath;
// Database child nodes
String presPath = "/pressure";
String tempPath = "/temperature";
String flowPath = "/flowrate";

FirebaseJson json;

//declare variables
float temperature_1, temperature_2, temperature_3, temperature_4, temperature_5, temperature_6;
float pressure1, pressure2, pressure3, pressure4, pressure5, pressure6;
float flow;

// Timer variables (send new readings every three minutes)
unsigned long sendDataPrevMillis = 0;
unsigned long timerDelay = 180000;

// Initialize WiFi
void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  initWiFi();

  // Assign the api key (required)
  config.api_key = API_KEY;

  // Assign the user sign in credentials
  //auth.user.email = USER_EMAIL;
  //auth.user.password = USER_PASSWORD; //Removed user email & password

  // Assign the RTDB URL (required)
  config.database_url = DATABASE_URL;

  Firebase.reconnectWiFi(true);
  fbdo.setResponseSize(4096);

  // Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  // Assign the maximum retry of token generation
  config.max_token_generation_retry = 5;

  // Initialize the library with the Firebase authen and config
  Firebase.begin(&config, &auth);

  // Getting the user UID might take a few seconds
  Serial.println("Getting User UID");
  while ((auth.token.uid) == "") {
    Serial.print('.');
    delay(1000);
  }
  // Print user UID
  uid = auth.token.uid.c_str();
  Serial.print("User UID: ");
  Serial.println(uid);

  // Update database path
  databasePath = "/UsersData/" + uid + "/readings";  
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
    sendData_Firebase();
    Serial.print(msg);
  }
}

void sendData_Firebase(){
  // Send new readings to database
  if (Firebase.ready() && (millis() - sendDataPrevMillis > timerDelay || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    parentPath= databasePath + "/" + String(millis());

    json.set(presPath.c_str(), String(pressure1));
    json.set(tempPath.c_str(), String(temperature_1));
    json.set(flowPath.c_str(), String(flow));
    Serial.printf("Set json... %s\n", Firebase.RTDB.setJSON(&fbdo, parentPath.c_str(), &json) ? "ok" : fbdo.errorReason().c_str());
  }
}
