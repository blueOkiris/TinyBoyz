/*
 * This file allows an UNO to monitor the display's data pins
 * to make sure it's functioning as expected
 * 
 * It's just for debug
 */

const int pinTinyWr = 13;
const int pinTinyCd = 12;
const int pinsTinyData[8] = { 11, 10, 9, 8, 7, 6, 5, 4 };

int lastClock = 0;

void setup() {
    pinMode(pinTinyWr, INPUT);
    pinMode(pinTinyCd, INPUT);
    for(int pin : pinsTinyData) {
        pinMode(pin, INPUT);
    }
    
    Serial.begin(9600);
    Serial.println(F("Monitoring AT Tinies"));
    
    lastClock = !digitalRead(pinTinyWr);
}

void loop() {
    if(lastClock != digitalRead(pinTinyWr)) {
        lastClock = digitalRead(pinTinyWr);
        printPins();
    }
}

void printPins() {
    Serial.print(F("\rClock: "));
    Serial.print(digitalRead(pinTinyWr) ? F("Off, ") : F("On,  "));
    Serial.print(digitalRead(pinTinyCd) ? F("Data,    ") : F("Command, "));
    Serial.print(F("Value: 0b "));
    for(int bit = 7; bit >= 5; bit--) {
        Serial.print(digitalRead(pinsTinyData[bit]));
    }
    Serial.print(F(" "));
    for(int bit = 4; bit >= 2; bit--) {
        Serial.print(digitalRead(pinsTinyData[bit]));
    }
    Serial.print(F(" "));
    for(int bit = 1; bit >= 0; bit--) {
        Serial.print(digitalRead(pinsTinyData[bit]));
    }
}
