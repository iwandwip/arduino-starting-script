#include "template-sensor.h"

SensorModule<uint16_t, NewPing *> ultra;

void setup() {
        Serial.begin(9600);
        ultra.init(&newpingInitCallback);
}

void loop() {
        ultra.update(&newpingHandlerCallback);
}
