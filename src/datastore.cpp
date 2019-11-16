#include "datastore.h"

uint16_t x_accel[COLLECTION_PERIOD*1000];
uint16_t y_accel[COLLECTION_PERIOD*1000];
uint16_t z_accel[COLLECTION_PERIOD*1000];

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
sensors_event_t event;
bool collecting_data;
unsigned long data_collect_start;

void start_data() {
    data_collect_start = millis();
    collecting_data = true;
}


void accel_loop() {
    if(!collecting_data)
        return;
    cur_time = millis();
    accel.getEvent(&event);

}
