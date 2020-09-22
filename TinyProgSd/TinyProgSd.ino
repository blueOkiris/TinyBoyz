#include "Fat16.hpp"

void setup() {
    if(!Fat16::init()) {
        while(1);
    }
}

void loop() {
    
}
