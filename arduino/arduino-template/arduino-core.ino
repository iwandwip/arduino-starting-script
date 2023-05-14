#include "sensor-module.h"
#include "MemoryFree.h"
#include "load-cell.h"

SensorModule sensor;
float value;

void setup() {
        Serial.begin(9600);
        sensor.addModule(new LoadCell(3, 4));
        sensor.init();
}

void loop() {
        Serial.print("freeMemory()= ");
        Serial.print(freeMemory());
        Serial.print(" | ");
        Serial.println(value);
        sensor.update(updateCallback);
}

void updateCallback() {
        sensor.getModule(0)->getSensorValue(&value);
}
