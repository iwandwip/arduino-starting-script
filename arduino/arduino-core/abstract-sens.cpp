/*
 *  abstract-sens.cpp
 *
 *  abstract sensor c
 *  Created on: 2023. 4. 3
 */

#include "abstract-sens.h"
#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define SENSOR_FILTER_KF 8

Abstract::Abstract()
  : sensorPin(A0) {
        isCalibrate = false;
        templateValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

Abstract::Abstract(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        templateValue = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

Abstract::~Abstract() {
        delete this;
}

void Abstract::init() {
        pinMode(sensorPin, INPUT);
}

void Abstract::update() {
        if (millis() - update_tm >= 500) {
                if (!isCalibrate) {
                        *templateValue = analogRead(sensorPin);
                        *templateValue = *templateValue * (5.0 / 1023.0);
                        *templateValue = *templateValue + (*templateValue * SENSOR_FILTER_KF);
                        *templateValue /= SENSOR_FILTER_KF + 1;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        templateValue[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        templateValue[SENS_RET_ACT_DATA] = templateValue[SENS_RET_RAW_DATA] + (templateValue[SENS_RET_RAW_DATA] * SENSOR_FILTER_KF);
                        templateValue[SENS_RET_ACT_DATA] /= SENSOR_FILTER_KF + 1;

                        templateValue[SENS_RET_AVG_DATA] = getSensorAverage(templateValue[SENS_RET_ACT_DATA]);
                        // templateValue[SENS_RET_FILTERED_DATA] = lowPassFilter(templateValue[SENS_RET_ACT_DATA], templateValue[SENS_RET_FILTERED_DATA], 0.1);
                        templateValue[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(templateValue[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }

                update_tm = millis();
        }
}

void Abstract::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| templateValueRaw: ");
                Serial.print(*templateValue);
                Serial.println();
                debug_tm = millis();
        }
}

void Abstract::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| templateValueRaw: ");
                Serial.print(templateValue[SENS_RET_RAW_DATA]);
                Serial.print("| templateValueAct: ");
                Serial.print(templateValue[SENS_RET_ACT_DATA]);
                Serial.print("| templateValueAvg: ");
                Serial.print(templateValue[SENS_RET_AVG_DATA]);
                Serial.print("| templateValueFiltered: ");
                Serial.print(templateValue[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void Abstract::getSensorValue(float* value) {
        *value = *templateValue;
}

void Abstract::getSensorValue(int* value) {
}

void Abstract::count() {
}

void Abstract::reset() {
}

float Abstract::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *templateValue;
        return templateValue[c];
}

void Abstract::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float Abstract::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float Abstract::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}