#ifndef CONFIG_H
#define CONFIG_H

#define AP_SSID "MECH366DAQ"
#define AP_PASSWORD "fourwordsalluppercase"

// Time to collect data (s)
#define COLLECTION_PERIOD 10

// Accelerometer chip I2C Address
#define ACCEL_ADDRESS 0x68

// Accelerometer interrupt pin
#define ACCEL_INT D5

// Accelerometer sample rate (Hz)
#define ACCEL_RATE 200

// Accelerometer offsets
#define ACCEL_X_OFST -3447
#define ACCEL_Y_OFST -4035
#define ACCEL_Z_OFST 915

// Gyro offsets
#define GYRO_X_OFST 23
#define GYRO_Y_OFST -29
#define GYRO_Z_OFST 18

// ESC "servo" pin
#define ESC_PIN D6

// Pulse width for ESC control (microseconds)
#define ESC_MIN_PULSE 1500 // Should be zero speed
#define ESC_MAX_PULSE 2000
#define ESC_NEG_PULSE 1000 // Should be max negative speed
#define ESC_JOG_LEVEL 0.5 // Percentage of max speed for jog
#define ESC_JOG_TIMEOUT 200 // Max timing interval between jog requests (ms)

#endif //CONFIG_H
