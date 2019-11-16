#ifndef DATASTORE_H
#define DATASTORE_H
#include <Arduino.h>
#include <FS.h>
#include <SparkFun_ADXL345.h>
#include "config.h"
#include "motordriver.h"

extern unsigned long data_collect_start;
extern ADXL345 accel;
extern bool collecting_data;
extern bool accel_need_save;

void start_data();
void stop_data();
void accel_loop();

#endif //DATASTORE_H
