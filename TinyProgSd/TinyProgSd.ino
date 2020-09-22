#include "Fat16.hpp"

void setup() {
    if(!Fat16::init()) {
        while(1);
    }

    if(!Fat16::openFile("README", "TXT")) {
        while(1);
    }

    while(!Fat16::eof()) {
        Fat16::readFile();

        for(int8_t i = 0; i < 32; i++) {
            fileBuffer[i];
        }
    }
}

void loop() {
    
}
