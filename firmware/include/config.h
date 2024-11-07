
// Stepper motor configuration

/* PINS */
#define STEP_PIN 36
#define DIR_PIN 35
#define ENABLE_PIN 41
#define MICROSTEP_1 40
#define MICROSTEP_2 39
#define MICROSTEP_3 38

#define MICROSTEPPING 1
#define STEPS_PER_REV 200
#define DIST_PER_REV 200

#define STEPS_PER_MM (STEPS_PER_REV * MICROSTEPPING / DIST_PER_REV)
#define STEPPER_REVERSED false
#define STEPPER_ENABLED false
#define STEPPER_DISABLED true

/* NEOPIXEL */
#define LED_PIN 2
#define PIXEL_COUNT 1

#define BRIGHTNESS 5
#define BOOTING CRGB::Yellow
#define SUCCESS CRGB::Green
#define ERROR CRGB::Red


/* BUTTONS */
#define UP_BUTTON_PIN 7
#define DOWN_BUTTON_PIN 8
#define SINGLE_BUTTON false

#define OPEN true
#define CLOSED false

/* TIME */
#define LATITUDE        48.1238978
#define LONGITUDE       11.5826488
#define DST_OFFSET      2

/* GPS */
#define RXD2 44
#define TXD2 43

#define EEPROM_SIZE 1
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */