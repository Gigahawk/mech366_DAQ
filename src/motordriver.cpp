#include "motordriver.h"

uint16_t input_buffer[COLLECTION_PERIOD*1000];
char loaded_file[32];

Servo esc;
bool motor_running;
unsigned long motor_cur_time;

void init_input_buffer() {
    Serial.println("Resetting input buffer");
    loaded_file[0] = '\0';
    for(int i=0; i < COLLECTION_PERIOD*1000; i++) {
        input_buffer[i] = 0;
    }
    Serial.println("Input buffer reset");
}

void start_motor(){
    motor_running = true;
}

uint8_t fillBuffer(String path) {
    Serial.println("fillBuffer: Opening " + path);
    char b;
    File f = SPIFFS.open(path, "r");
    if (!f) {
        Serial.println("file open failed");
        return FILE_LOAD_FAIL;
    }
    for(int i=0; i < COLLECTION_PERIOD*1000; i++) {
        if(!f.readBytes(&b, 1)) {
            Serial.println("File not long enough");
            init_input_buffer();
            return FILE_SIZE_ERR;
        }
        input_buffer[i] = map(b, 0, 255, ESC_MIN_PULSE, ESC_MAX_PULSE);
        //Serial.print("Read raw value ");
        //Serial.print(b);
        //Serial.print(" as ");
        //Serial.print(input_buffer[i]);
        //Serial.print(" to index ");
        //Serial.println(i);
    }
    path.toCharArray(loaded_file, 32);
    Serial.println("Loaded successfuly");
    return LOAD_SUCCESS;
}

void motor_loop() {
    motor_cur_time = millis() - data_collect_start;
    if(motor_running){
        if(motor_cur_time > COLLECTION_PERIOD*1000) {
            stop_data();
            motor_running = false;
            return;
        }
        esc.writeMicroseconds(input_buffer[motor_cur_time]);
    }
    esc.writeMicroseconds(ESC_MIN_PULSE);
}
