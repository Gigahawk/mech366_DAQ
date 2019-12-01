#ifndef DATASTORE_H
#define DATASTORE_H
#include <Arduino.h>
#include <FS.h>
#include <MPU6050.h>
#include "config.h"
#include "motordriver.h"

extern unsigned long data_collect_start;
extern MPU6050 accel;
extern bool imu_connected;
extern bool collecting_data;
extern bool accel_need_save;

void start_data();
void stop_data();
void accel_loop();

#endif //DATASTORE_H
