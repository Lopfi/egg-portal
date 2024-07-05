#include "defines.h"
#include "Credentials.h"
#include "dynamicParams.h"
#include "config.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP32Time.h>

//ESP32Time rtc;
//TODO: set offset acording to timezone
//TODO: make timezone configurable in web interface or config file
ESP32Time rtc(3600);  // offset in seconds GMT+1

ESPAsync_WiFiManager_Lite* ESPAsync_WiFiManager;

//AsyncWebServer server(81);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

int distance = 20; // opening distance in mm
bool state = OPEN; // current door state

void setupPins();
void setDoor(bool setState);

void displayCredentials();
void displayCredentialsInLoop();
void heartBeatPrint();
void check_status();
void update_time();

#if USING_CUSTOMS_STYLE
const char NewCustomsStyle[] PROGMEM =
  "<style>div,input{padding:5px;font-size:1em;}input{width:95%;}body{text-align: center;}"\
  "button{background-color:blue;color:white;line-height:2.4rem;font-size:1.2rem;width:100%;}fieldset{border-radius:0.3rem;margin:0px;}</style>";
#endif

void setup()
{
  // Debug console
  Serial.begin(115200);
  while (!Serial);

  delay(200);

  Serial.print(F("\nStarting ESPAsync_WiFi using "));
  Serial.print(FS_Name);
  Serial.print(F(" on "));
  Serial.println(ARDUINO_BOARD);
  Serial.println(ESP_ASYNC_WIFI_MANAGER_LITE_VERSION);
  //Serial.println(ESP_MULTI_RESET_DETECTOR_VERSION);

  ESPAsync_WiFiManager = new ESPAsync_WiFiManager_Lite();
  String AP_SSID = "EGGPortal";
  String AP_PWD  = "chickenchicken";
  
  // Set customized AP SSID and PWD
  ESPAsync_WiFiManager->setConfigPortal(AP_SSID, AP_PWD);

  // Optional to change default AP IP(192.168.4.1) and channel(10)
  //ESPAsync_WiFiManager->setConfigPortalIP(IPAddress(192, 168, 120, 1));
  ESPAsync_WiFiManager->setConfigPortalChannel(0);

#if USING_CUSTOMS_STYLE
  ESPAsync_WiFiManager->setCustomsStyle(NewCustomsStyle);
#endif

#if USING_CUSTOMS_HEAD_ELEMENT
  ESPAsync_WiFiManager->setCustomsHeadElement(PSTR("<style>html{filter: invert(10%);}</style>"));
#endif

  // Set customized DHCP HostName
  ESPAsync_WiFiManager->begin(HOST_NAME);


  setupPins();
  //ElegantOTA.begin(&server);    // Start ElegantOTA
  //server.begin();
}



void loop()
{
  //Serial.println(rtc.getTime("%A, %B %d %Y %H:%M:%S"));   // (String) returns time with specified format 
//TODO: calculate sunrise and sunset 
//TODO: update time
  ESPAsync_WiFiManager->run();
  check_status();

  displayCredentialsInLoop();
  if (!ESPAsync_WiFiManager->isConfigMode())
    ElegantOTA.loop();  

}


void update_time()
{  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Wifi not connected! Can't update time.");        // H means connected to WiFi
    return;
  }
  Serial.println("Updating time...");
  timeClient.update(); // get time from ntp
  Serial.println(timeClient.getFormattedTime());
  rtc.setTime(timeClient.getEpochTime(), 0); // set time to rtc
  //rtc.setTime(30, 24, 15, 17, 1, 2021);  // 17th Jan 2021 15:24:30
  Serial.println("Time updated");
}

void setupPins() {
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(resetButtonPin, INPUT);
  pinMode(neoPixelPin, OUTPUT);

  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(neoPixelPin, LOW);
}

void setDoor(bool setState) {
  if (setState == state) return;

  digitalWrite(DIR_PIN, STEPPER_REVERSED ? !setState : setState);
  digitalWrite(ENABLE_PIN, STEPPER_ENABLED);
  for (int i = 0; i < STEPS_PER_MM * distance; i++) {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(1000);
  }
  digitalWrite(ENABLE_PIN, STEPPER_DISABLED);
  state = setState;
}

void displayCredentialsInLoop()
{
  static bool displayedCredentials = false;

  if (!displayedCredentials)
  {
    for (int i = 0; i < NUM_MENU_ITEMS; i++)
    {
      if (!strlen(myMenuItems[i].pdata))
      {
        break;
      }

      if ( i == (NUM_MENU_ITEMS - 1) )
      {
        displayedCredentials = true;
        displayCredentials();
      }
    }
  }
}

void displayCredentials()
{
  Serial.println(F("\nYour stored Credentials :"));

  for (uint16_t i = 0; i < NUM_MENU_ITEMS; i++)
  {
    Serial.print(myMenuItems[i].displayName);
    Serial.print(F(" = "));
    Serial.println(myMenuItems[i].pdata);
  }
}

void heartBeatPrint()
{
  static int num = 1;

  if (WiFi.status() == WL_CONNECTED)
    Serial.print("H");        // H means connected to WiFi
  else
  {
    if (ESPAsync_WiFiManager->isConfigMode())
      Serial.print("C");        // C means in Config Mode
    else
      Serial.print("F");        // F means not connected to WiFi
  }

  if (num == 80)
  {
    Serial.println();
    num = 1;
  }
  else if (num++ % 10 == 0)
  {
    Serial.print(F(" "));
  }
}

void check_status()
{
  static unsigned long checkstatus_timeout = 0;

  //KH
#define HEARTBEAT_INTERVAL    20000L
  // Print hearbeat every HEARTBEAT_INTERVAL (20) seconds.
  if ((millis() > checkstatus_timeout) || (checkstatus_timeout == 0))
  {
    heartBeatPrint();
    checkstatus_timeout = millis() + HEARTBEAT_INTERVAL;
  }
}