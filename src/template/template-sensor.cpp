#include "template-sensor.h"
#include "HardwareSerial.h"
#include "MemoryFree.h"
#include "Arduino.h"

template<typename dtype, typename cPtr>
SensorModule<dtype, cPtr>::SensorModule() {
}

template<typename dtype, typename cPtr>
SensorModule<dtype, cPtr>::~SensorModule() {
        delete sensorClass;
        delete[] data;
        delete this;
}

template<typename dtype, typename cPtr>
void SensorModule<dtype, cPtr>::init(CallbackFunc<cPtr> callback) {
        sensorClass = (*callback)(sensorClass);
}

template<typename dtype, typename cPtr>
void SensorModule<dtype, cPtr>::update(CallbackFuncUpdate<dtype*, cPtr> callback) {
        (*callback)(data, sensorClass);
}

template<typename dtype, typename cPtr>
cPtr SensorModule<dtype, cPtr>::classes() {
        return sensorClass;
}

template<typename dtype, typename cPtr>
dtype* SensorModule<dtype, cPtr>::value() {
        return data;
}

// callback function
template<typename T>
T newpingInitCallback(T packet) {
        return new NewPing(3, 2, 200);
}
template<typename T, typename U>
void newpingHandlerCallback(T packet, U classses) {
        static uint32_t tmr;
        if (millis() - tmr >= 100) {
                // memory monitoring to avoid memory leak
                Serial.print("freeMemory()=");
                Serial.print(freeMemory());
                Serial.print(" | ultra: ");
                packet[ACT_DATA] = classses->ping_cm();
                Serial.println(packet[ACT_DATA]);
                tmr = millis();
        }
}

// extern class & function, gcc ver < C++17 (C++11 / C++14)
template NewPing* newpingInitCallback<NewPing*>(NewPing* packet);
template void newpingHandlerCallback<uint16_t*, NewPing*>(uint16_t* packet, NewPing* classes);
template class SensorModule<uint16_t, NewPing*>;