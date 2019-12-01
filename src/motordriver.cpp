#include "motordriver.h"

#define ESC_JOG_FORWARDS (int)(ESC_JOG_LEVEL*(ESC_MAX_PULSE - ESC_MIN_PULSE))
#define ESC_JOG_BACKWARDS (int)(ESC_JOG_LEVEL*(ESC_MIN_PULSE - ESC_NEG_PULSE))

uint16_t input_buffer[COLLECTION_PERIOD*1000];
char loaded_file[32];

Servo esc;
bool motor_running;
bool motor_jogging;
bool jog_forwards;
unsigned long motor_jog_start;
unsigned long motor_jog_time;
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

void start_jog(bool forwards) {
    motor_jog_start = millis();
    jog_forwards = forwards;
    motor_jogging = true;
}

void stop_jog() {
    motor_jogging = false;
}

void motor_loop() {
    if(motor_running){
        motor_cur_time = millis() - data_collect_start;
        if(motor_cur_time > COLLECTION_PERIOD*1000) {
            stop_data();
            motor_running = false;
            return;
        }
        esc.writeMicroseconds(input_buffer[motor_cur_time]);
    } else if(motor_jogging) {
        motor_jog_time = millis() - motor_jog_start;
        if(motor_jog_time > ESC_JOG_TIMEOUT) {
            esc.writeMicroseconds(ESC_MIN_PULSE);
            motor_jogging = false;
            return;
        }
        if(jog_forwards) {
            esc.writeMicroseconds(ESC_JOG_FORWARDS);
        } else {
            esc.writeMicroseconds(ESC_JOG_BACKWARDS);
        }
    } else {
        esc.writeMicroseconds(ESC_MIN_PULSE);
    }

}
