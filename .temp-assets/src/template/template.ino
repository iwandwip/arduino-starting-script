#include "template-sensor.h"
#include "hc-sr04.h"

// SensorModule<uint16_t, NewPing *> ultra;
HCsr04 sonar, ultra;

void setup() {
        Serial.begin(9600);
        // ultra.init(&newpingInitCallback);

        // sonar[0].init();
        // sonar[1].init();

        // void *data[] = { &sonar[0], &sonar[1] };

        // for (int j = 0; j < 2; j++) {
        //         auto dataPtr = reinterpret_cast<decltype(data[0])>(data[j]);
        // }

        AbstractSens* sens[] = { &sonar, &ultra };
        for (int i = 0; i < 2; i++) {
                AbstractSens* sensorPtr = reinterpret_cast<AbstractSens*>(sens[i]);
                sensorPtr->update();
        }

        // int i = 42;
        // float f = 3.14;
        // char c = 'a';

        // void* myData[] = { &i, &f, &c };

        // for (int j = 0; j < 3; j++) {
        //         auto dataPtr = reinterpret_cast<decltype(myData[0])>(myData[j]);
        // }
}

void loop() {
        // ultra.update(&newpingHandlerCallback);
}
