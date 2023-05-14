/*
 *  lora-com.h
 *
 *  lora communication c
 *  Created on: 2023. 4. 3
 */

#include "lora-com.h"

LoRaModule::LoRaModule() {
}

LoRaModule::~LoRaModule() {
}

void LoRaModule::init() {
        LoRa.setPins(5, 14, 2);
        if (!LoRa.begin(915E6))
                while (1) Serial.print(".");
        else Serial.print("LoRa initialize");
        // LoRa.onReceive(onReceive);
        // LoRa.receive();
}

template<typename T>
void LoRaModule::addData(T value) {
        data += String(value);
        data += SEPARATOR;
}

void LoRaModule::clearData() {
        data = NONE;
}

void LoRaModule::sendData(uint32_t __time) {
        if (millis() - sendTime >= __time) {
                LoRa.beginPacket();
                LoRa.print(data);
                LoRa.endPacket(true);  // true = async
                // LoRa.receive();
                Serial.println(data);
                sendTime = millis();
        }
}

void LoRaModule::receive(void (*onReceive)(String)) {
        if (onReceive == nullptr) return;
        int packetSize = LoRa.parsePacket();
        if (packetSize) {
                String data;
                while (LoRa.available()) {
                        data += (char)LoRa.read();
                }
                onReceive(data);
        }
}

float LoRaModule::getData(String data, uint8_t index) {
        return parseStr(data, ";", index).toFloat();
}

String LoRaModule::parseStr(String data, char separator[], int index) {
        int found = 0;
        int strIndex[] = { 0, -1 };
        int maxIndex = data.length() - 1;
        for (int i = 0; i <= maxIndex && found <= index; i++) {
                if (data.charAt(i) == separator[0] || i == maxIndex) {
                        found++;
                        strIndex[0] = strIndex[1] + 1;
                        strIndex[1] = (i == maxIndex) ? i + 1 : i;
                }
        }
        return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}