#include "base-hardware.h"
#include <MemoryFree.h>

#define SERIAL_TIMEOUT 2000

HardwareModule::HardwareModule()
  : len(2), data(""), separator(';'), isReceive(false) {
}

HardwareModule::~HardwareModule() {
        delete this;
}

void HardwareModule::init() {
        // extDataPtr = new ExternData;
}

void HardwareModule::update() {
        if (Serial.available()) {
                String buffPtr = Serial.readStringUntil('\n');
                if (buffPtr.length() > 10) {
                        String diff = parseStr(buffPtr, ":", 0);
                        String dataG = parseStr(buffPtr, ":", 1);
                        if (diff == "G1") {
                                eData->kpGains = parseStr(dataG, ";", 0).toFloat();
                                eData->kiGains = parseStr(dataG, ";", 1).toFloat();
                                eData->kdGains = parseStr(dataG, ";", 2).toFloat();
                                eData->tdGains = parseStr(dataG, ";", 3).toFloat();
                        } else if (diff == "G2") {
                                eData->servoOil = parseStr(dataG, ";", 0).toInt();
                                eData->servoWater = parseStr(dataG, ";", 1).toInt();
                                eData->setPointOil = parseStr(dataG, ";", 2).toFloat();
                                eData->setPointWater = parseStr(dataG, ";", 3).toFloat();
                        } else if (diff == "G3") {
                                eData->mode = parseStr(dataG, ";", 0).toInt();
                                eData->autoStart = parseStr(dataG, ";", 1).toInt();
                                eData->manualStart = parseStr(dataG, ";", 2).toInt();
                                eData->eReset = parseStr(dataG, ";", 3).toInt();
                                eData->relayOil = parseStr(dataG, ";", 4).toInt();
                                eData->relayWater = parseStr(dataG, ";", 5).toInt();
                        } else if (diff == "G4") {
                                eData->feedOil = parseStr(dataG, ";", 0).toFloat();
                                eData->feedWater = parseStr(dataG, ";", 1).toFloat();
                        }
                }
        }
}

void HardwareModule::debug() {
}

void HardwareModule::calibrate() {
}

void HardwareModule::setSeparator(char sep) {
        separator = sep;
}

void HardwareModule::setData(ExternData* dataPtr) {
        eData = dataPtr;
}

float HardwareModule::getFloatData(uint8_t index) {
        if (read) return parseStr(data, separator, index).toFloat();
}

int HardwareModule::getIntData(uint8_t index) {
        if (read) return parseStr(data, separator, index).toInt();
}

bool HardwareModule::getBoolData(uint8_t index) {
        if (read) return parseStr(data, separator, index).toInt();
}

String HardwareModule::getStringData(uint8_t index) {
        if (read) return parseStr(data, separator, index);
}

void HardwareModule::getFloatData(float* val, uint8_t index) {
        if (read) *val = parseStr(data, separator, index).toFloat();
}

void HardwareModule::getIntData(int* val, uint8_t index) {
        if (read) *val = parseStr(data, separator, index).toInt();
}

void HardwareModule::getBoolData(bool* val, uint8_t index) {
        if (read) *val = parseStr(data, separator, index).toInt();
}

void HardwareModule::getStringData(String* val, uint8_t index) {
        if (read) *val = parseStr(data, separator, index);
}

String HardwareModule::parseStr(String dataIn, char separator[], int index) {
        int found = 0;
        int strIndex[] = { 0, -1 };
        int maxIndex = dataIn.length() - 1;
        for (int i = 0; i <= maxIndex && found <= index; i++) {
                if (dataIn.charAt(i) == separator[0] || i == maxIndex) {
                        found++;
                        strIndex[0] = strIndex[1] + 1;
                        strIndex[1] = (i == maxIndex) ? i + 1 : i;
                }
        }
        return found > index ? dataIn.substring(strIndex[0], strIndex[1]) : "";
}
