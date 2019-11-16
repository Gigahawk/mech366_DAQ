#ifndef CONFIG_H
#define CONFIG_H

#define AP_SSID "MECH366DAQ"
#define AP_PASSWORD "fourwordsalluppercase"

// Time to collect data (s)
#define COLLECTION_PERIOD 10

// Accelerometer chip select pin
#define ACCEL_CS D1

// Accelerometer interrupt pin
#define ACCEL_INT D3

// Accelerometer sample rate (Hz)
#define ACCEL_RATE 100

// ESC "servo" pin
#define ESC_PIN D2

// Pulse width for ESC control (microseconds)
#define ESC_MIN_PULSE 1500 // Should be zero speed
#define ESC_MAX_PULSE 2000 

#endif //CONFIG_H
