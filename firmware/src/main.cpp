#include <Arduino.h>
#include <FastLED.h>
#include <sunset.h>
#include <TimeLib.h>
#include <TinyGPSPlus.h>
#include <EEPROM.h>

#include "config.h"

SunSet sun;
CRGB leds[PIXEL_COUNT];
TinyGPSPlus gps;

int distance = 20; // opening distance in mm
bool state; // current door state

void setupPins();
void setDoor(bool setState);
void updateTime();
void digitalClockDisplay();
void printDigits(int digits);
void displayInfo();

void setup()
{
  // Fast LED
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, PIXEL_COUNT);
  FastLED.setBrightness(5);
  leds[0] = BOOTING;
  FastLED.show();

  // Debug console
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  while (!Serial);
  delay(200);
  Serial.println("Starting up...");
  EEPROM.begin(EEPROM_SIZE);
  
  state = EEPROM.read(0);

  Serial.print("Door state: ");
  Serial.println(state ? "OPEN" : "CLOSED");

  setupPins();

  do {
    Serial.println("Waiting for GPS signal...");
    if (Serial1.available()) {
      if (gps.encode(Serial1.read())) { // process gps messages
        leds[0] = SUCCESS;
        FastLED.show();
        // when TinyGPS reports new data...
        if (gps.date.isValid() && gps.time.isValid() && gps.location.isValid() && gps.time.age() <500)
        {
          // set the Time to the latest GPS reading
          setTime(gps.time.hour(), gps.time.minute(), gps.time.second(), gps.date.day(), gps.date.month(), gps.date.year());
          adjustTime(DST_OFFSET * SECS_PER_HOUR);
          sun.setPosition(gps.location.lat(), gps.location.lng(), DST_OFFSET);
          sun.setCurrentDate(year(), month(), day());
        }
        else {
          Serial.print(F("INVALID"));
          leds[0] = ERROR;
          FastLED.show();
        }
      }
    }
  } while (timeStatus() == timeNotSet || year() < 2000);
  
  updateTime();

  int now = hour() * 60 + minute();
  int nextWake;

  if (state == OPEN) {
    nextWake = static_cast<int>(sun.calcSunrise());
  }
  else {
    nextWake = static_cast<int>(sun.calcSunset());
  }

  state = !state;
  setDoor(state);
  EEPROM.write(0, state);
  EEPROM.commit();

  Serial.println("Going to sleep for " + String(nextWake - now) + " minutes");


  int sleepTime = (nextWake - now) * 60; // sleep time in seconds

  esp_sleep_enable_timer_wakeup(sleepTime * uS_TO_S_FACTOR);
}

void loop()
{
}

void setupPins() {
  //Stepper motor
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(MICROSTEP_1, OUTPUT);
  pinMode(MICROSTEP_2, OUTPUT);
  pinMode(MICROSTEP_3, OUTPUT);
  //Buttons
  pinMode(UP_BUTTON_PIN, INPUT);
  pinMode(DOWN_BUTTON_PIN, INPUT);

  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);
  digitalWrite(ENABLE_PIN, HIGH);
  digitalWrite(MICROSTEP_1, LOW);
  digitalWrite(MICROSTEP_2, LOW);
  digitalWrite(MICROSTEP_3, LOW);
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

void updateTime() {
  digitalClockDisplay();
  int sunrise = static_cast<int>(sun.calcSunrise());
  int sunset = static_cast<int>(sun.calcSunset());  
  char buffer[100];
  sprintf(buffer, "Sunrise at %d:%dam, Sunset at %d:%dpm", sunrise / 60, sunrise % 60, sunset / 60, sunset % 60);
  Serial.println(buffer);

}

void printDigits(int digits, char separator) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(separator);
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute(), ':');
  printDigits(second(), ':');
  printDigits(day(), ' ');
  printDigits(month(), '.');
  printDigits(year(), '.');
  Serial.println(); 
}
