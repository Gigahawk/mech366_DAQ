#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H
#define LOAD_SUCCESS 0
#define FILE_LOAD_FAIL 1
#define FILE_SIZE_ERR 2
#include <Arduino.h>
#include <FS.h>
#include <Servo.h>
#include "config.h"
#include "datastore.h"



extern Servo esc;
extern unsigned long data_collect_start;
extern uint16_t input_buffer[];
extern char loaded_file[];
extern bool motor_running;

uint8_t fillBuffer(String path);
void start_motor();
void motor_loop();
void start_jog(bool forwards);
void stop_jog();

#endif //MOTORDRIVER_H
