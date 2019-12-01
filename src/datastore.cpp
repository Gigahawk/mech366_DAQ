#include "datastore.h"
#define DS_BUFFER_LENGTH (uint16_t)(1.05*COLLECTION_PERIOD*ACCEL_RATE)

int16_t x_accel[DS_BUFFER_LENGTH];
int16_t y_accel[DS_BUFFER_LENGTH];
int16_t z_accel[DS_BUFFER_LENGTH];
int16_t accel_time[DS_BUFFER_LENGTH];
volatile uint16_t accel_idx;

MPU6050 accel = MPU6050(ACCEL_ADDRESS);
bool collecting_data;
bool accel_need_save;
bool imu_connected = false;
unsigned long data_collect_start;
volatile unsigned long data_cur_time;

void accel_isr();

void reset_data() {
    Serial.println("Clearing accelerometer buffers");
    for(int i=0; i < DS_BUFFER_LENGTH; i++) {
        x_accel[i] = 0;
        y_accel[i] = 0;
        z_accel[i] = 0;
    }
    Serial.println("Accelerometer buffers cleared");
}

void start_data() {
    Serial.println("Starting data collection");
    accel_idx = 0;
    data_collect_start = millis();
    attachInterrupt(
            digitalPinToInterrupt(ACCEL_INT),
            accel_isr,
            FALLING);
    accel_isr();
}

String generateFilename() {
    String filename = String(loaded_file) + "_" + String(millis()) + ".xyz";
    // Try again if filename already exists
    while(SPIFFS.exists(filename))
        filename = String(loaded_file) + "_" + String(millis()) + ".xyz";

    return filename;
}

void save_data() {
    String filename = generateFilename();
    Serial.print("Saving data to ");
    Serial.println(filename);
    int16_t txyz[4];

    File xyz_file = SPIFFS.open(filename, "w");
    if(!xyz_file) {
        Serial.println("file open failed");
        return;
    }
    for(int i=0; i < DS_BUFFER_LENGTH; i++) {
        txyz[0] = accel_time[i];
        txyz[1] = x_accel[i];
        txyz[2] = y_accel[i];
        txyz[3] = z_accel[i];
        xyz_file.write((uint8_t*)txyz, 8);
    }
    xyz_file.close();
    Serial.print("Saved successfully");
}

void stop_data() {
    detachInterrupt(digitalPinToInterrupt(ACCEL_INT));
    accel_need_save = true;
}

void accel_loop() {
    if(accel_need_save) {
        save_data();
        accel_need_save = false;
    }
}

void ICACHE_RAM_ATTR accel_isr() {
    data_cur_time = millis() - data_collect_start;
    if(data_cur_time >= COLLECTION_PERIOD*1000 || accel_idx >= DS_BUFFER_LENGTH) {
        stop_data();
        return;
    }

    accel.getAcceleration(x_accel + accel_idx, y_accel + accel_idx, z_accel + accel_idx);
    accel_time[accel_idx] = data_cur_time;
    accel_idx++;
}
