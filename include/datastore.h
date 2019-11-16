#ifndef DATASTORE_H
#define DATASTORE_H
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include "config.h"

extern Adafruit_ADXL345_Unified accel;
extern sensors_event_t event;
extern bool collecting_data;

void start_data();
void accel_loop();

#endif //DATASTORE_H
