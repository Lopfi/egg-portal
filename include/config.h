
// Stepper motor configuration

/* PINS */
#define stepPin 2
#define dirPin 3
#define enablePin 4

#define microstepping 1
#define stepsPerRevolution 200
#define distancePerRevolution 200

#define stepsPerMillimeter (stepsPerRevolution * microstepping / distancePerRevolution)
#define REVERSED false
#define ENABLED false
#define DISABLED true

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