/****************************************************************************************************************************
  defines.h
  For ESP8266 / ESP32 boards

  ESPAsync_WiFiManager_Lite (https://github.com/khoih-prog/ESPAsync_WiFiManager_Lite) is a library
  for the ESP32/ESP8266 boards to enable store Credentials in EEPROM/SPIFFS/LittleFS for easy
  configuration/reconfiguration and autoconnect/autoreconnect of WiFi and other services without Hardcoding.

  Built by Khoi Hoang https://github.com/khoih-prog/ESPAsync_WiFiManager_Lite
  Licensed under MIT license
 *****************************************************************************************************************************/

/* Comment this out to disable prints and save space */
#define ESP_WM_LITE_DEBUG_OUTPUT      Serial

#define _ESP_WM_LITE_LOGLEVEL_        2

// use builtin LED to show configuration mode
#define USE_LED_BUILTIN               true

#define USING_MRD                     false
#define MULTIRESETDETECTOR_DEBUG      true


/////////////////////////////////////////////

// LittleFS has higher priority than SPIFFS
#if ( defined(ESP_ARDUINO_VERSION_MAJOR) && (ESP_ARDUINO_VERSION_MAJOR >= 2) )
  #define USE_LITTLEFS          true
  #define USE_SPIFFS            false
#elif defined(ARDUINO_ESP32C3_DEV)
  // For core v1.0.6-, ESP32-C3 only supporting SPIFFS and EEPROM. To use v2.0.0+ for LittleFS
  #define USE_LITTLEFS          false
  #define USE_SPIFFS            true
#else
  #define USE_LITTLEFS          true
  #define USE_SPIFFS            false  
#endif

/////////////////////////////////////////////

// Add customs headers from v1.2.0
#define USING_CUSTOMS_STYLE           true
#define USING_CUSTOMS_HEAD_ELEMENT    false
#define USING_CORS_FEATURE            false

/////////////////////////////////////////////

// Force some params
#define TIMEOUT_RECONNECT_WIFI                    10000L

// Permit running CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET times before reset hardware
// to permit user another chance to config. Only if Config Data is valid.
// If Config Data is invalid, this has no effect as Config Portal will persist
#define RESET_IF_CONFIG_TIMEOUT                   true

// Permitted range of user-defined CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET between 2-100
#define CONFIG_TIMEOUT_RETRYTIMES_BEFORE_RESET    5

// Config Timeout 120s (default 60s). Applicable only if Config Data is Valid
#define CONFIG_TIMEOUT                            120000L

/////////////////////////////////////////////

// Permit input only one set of WiFi SSID/PWD. The other can be "NULL or "blank"
// Default is false (if not defined) => must input 2 sets of SSID/PWD
#define REQUIRE_ONE_SET_SSID_PW               true    //false

// Max times to try WiFi per loop() iteration. To avoid blocking issue in loop()
// Default 1 if not defined, and minimum 1.
#define MAX_NUM_WIFI_RECON_TRIES_PER_LOOP     1

// Default no interval between recon WiFi if lost
// Max permitted interval will be 10mins
// Uncomment to use. Be careful, WiFi reconnect will be delayed if using this method
// Only use whenever urgent tasks in loop() can't be delayed. But if so, it's better you have to rewrite your code, e.g. using higher priority tasks.
#define WIFI_RECON_INTERVAL                   30000

/////////////////////////////////////////////

// Permit reset hardware if no WiFi to permit user another chance to access Config Portal.
#define RESET_IF_NO_WIFI              false

/////////////////////////////////////////////

#define SCAN_WIFI_NETWORKS                  true

// To be able to manually input SSID, not from a scanned SSID lists
#define MANUAL_SSID_INPUT_ALLOWED           true

// From 2-15
#define MAX_SSID_IN_LIST                  8

/////////////////////////////////////////////

// Optional, to use Board Name in Menu
#define USING_BOARD_NAME                    false

/////////////////////////////////////////////

#include <ESPAsync_WiFiManager_Lite.h>

#define HOST_NAME   "EGG-Portal"

#define LED_PIN     LED_BUILTIN