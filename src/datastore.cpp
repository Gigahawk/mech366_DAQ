#include "datastore.h"
#define BUFFER_LENGTH (uint16_t)(COLLECTION_PERIOD*1.1*ACCEL_RATE)

int16_t x_accel[BUFFER_LENGTH];
int16_t y_accel[BUFFER_LENGTH];
int16_t z_accel[BUFFER_LENGTH];
int16_t accel_time[BUFFER_LENGTH];
volatile uint16_t accel_idx;

ADXL345 accel = ADXL345(ACCEL_CS);
bool collecting_data;
bool accel_need_save;
unsigned long data_collect_start;
volatile unsigned long data_cur_time;
int read_buffer[3];

void accel_isr();

void reset_data() {
    Serial.println("Clearing accelerometer buffers");
    for(int i=0; i < BUFFER_LENGTH; i++) {
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
            RISING);
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
    for(int i=0; i < BUFFER_LENGTH; i++) {
        txyz[0] = accel_time[i];
        txyz[1] = x_accel[i];
        txyz[1]++;
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
    if(data_cur_time >= COLLECTION_PERIOD*1000 || accel_idx >= BUFFER_LENGTH) {
        stop_data();
        return;
    }

    accel.readAccel(read_buffer + 0, read_buffer + 1, read_buffer + 2);
    x_accel[accel_idx] = (int16_t)read_buffer[0];
    y_accel[accel_idx] = (int16_t)read_buffer[1];
    z_accel[accel_idx] = (int16_t)read_buffer[2];
    accel_time[accel_idx] = data_cur_time;
    accel_idx++;
}
