
// Stepper motor configuration

/* PINS */
#define STEP_PIN 2
#define DIR_PIN 3
#define ENABLE_PIN 4

#define MICROSTEPPING 1
#define STEPS_PER_REV 200
#define DIST_PER_REV 200

#define STEPS_PER_MM (STEPS_PER_REV * MICROSTEPPING / DIST_PER_REV)
#define STEPPER_REVERSED false
#define STEPPER_ENABLED false
#define STEPPER_DISABLED true

/* RESET BUTTON */
#define resetButtonPin 5

/* NEOPIXEL */
#define neoPixelPin 6
#define neoPixelCount 1

/* BUTTONS */
#define upButtonPin 7
#define downButtonPin 8
#define singleButton false

#define OPEN true
#define CLOSED false

//////////////////////////////
/// Double Reset Detector ///
//////////////////////////////

// Number of seconds after reset during which a
// subseqent reset will be considered a double reset.
#define DRD_TIMEOUT 10

// RTC Memory Address for the DoubleResetDetector to use
#define DRD_ADDRESS 0

// To not display stored SSIDs and PWDs on Config Portal, select false. Default is true
// Even the stored Credentials are not display, just leave them all blank to reconnect and reuse the stored Credentials 
//#define DISPLAY_STORED_CREDENTIALS_IN_CP        false

// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _ESPASYNC_WIFIMGR_LOGLEVEL_    4