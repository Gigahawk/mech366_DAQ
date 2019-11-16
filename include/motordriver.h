#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H
#define LOAD_SUCCESS 0
#define FILE_LOAD_FAIL 1
#define FILE_SIZE_ERR 2
#include <Arduino.h>
#include <FS.h>
#include <Servo.h>
#include "config.h"



extern char input_buffer[];
extern char loaded_file[];

uint8_t fillBuffer(String path);

#endif //MOTORDRIVER_H
