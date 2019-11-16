#include "motordriver.h"

char input_buffer[COLLECTION_PERIOD*1000];
char loaded_file[32];

void init_input_buffer() {
    loaded_file[0] = '\0';
    for(int i=0; i < COLLECTION_PERIOD*1000; i++) {
        input_buffer[i] = 0;
    }
}

uint8_t fillBuffer(String path) {
    Serial.println("fillBuffer: Opening " + path);
    File f = SPIFFS.open(path, "r");
    if (!f) {
        Serial.println("file open failed");
        return FILE_LOAD_FAIL;
    }
    for(int i=0; i < COLLECTION_PERIOD*1000; i++) {
        if(!f.readBytes(input_buffer + i, 1)) {
            Serial.println("File not long enough");
            init_input_buffer();
            return FILE_SIZE_ERR;
        }
        Serial.print("Read value ");
        Serial.print((uint8_t)input_buffer[i]);
        Serial.print(" to index ");
        Serial.println(i);
    }
    path.toCharArray(loaded_file, 32);
    return LOAD_SUCCESS;
}
