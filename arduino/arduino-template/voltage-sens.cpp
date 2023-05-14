/*
 *  voltage-sens.cpp
 *
 *  voltage sensor c
 *  Created on: 2023. 4. 3
 */

#include "voltage-sens.h"
#include "Arduino.h"
#include "SimpleKalmanFilter.h"

#define VOLTAGE_FILTER_KF 8

Voltage::Voltage()
  : sensorPin(A0) {
        isCalibrate = false;
        voltage = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

Voltage::Voltage(uint8_t __pin, bool enableCalibrate) {
        this->sensorPin = __pin;
        isCalibrate = enableCalibrate;
        voltage = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

Voltage::~Voltage() {
        delete this;
}

void Voltage::init() {
        pinMode(sensorPin, INPUT);
}

void Voltage::update() {
        if (millis() - update_tm >= 500) {
                if (!isCalibrate) {
                        *voltage = analogRead(sensorPin);
                        *voltage = *voltage * (5.0 / 1023.0);
                        *voltage = *voltage + (*voltage * VOLTAGE_FILTER_KF);
                        *voltage /= VOLTAGE_FILTER_KF + 1;
                } else {
                        SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                        voltage[SENS_RET_RAW_DATA] = analogRead(sensorPin);
                        voltage[SENS_RET_ACT_DATA] = voltage[SENS_RET_RAW_DATA] + (voltage[SENS_RET_RAW_DATA] * VOLTAGE_FILTER_KF);
                        voltage[SENS_RET_ACT_DATA] /= VOLTAGE_FILTER_KF + 1;

                        voltage[SENS_RET_AVG_DATA] = getSensorAverage(voltage[SENS_RET_ACT_DATA]);
                        // voltage[SENS_RET_FILTERED_DATA] = lowPassFilter(voltage[SENS_RET_ACT_DATA], voltage[SENS_RET_FILTERED_DATA], 0.1);
                        voltage[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(voltage[SENS_RET_ACT_DATA]);
                        delete sonarKf;
                }

                update_tm = millis();
        }
}

void Voltage::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| voltageRaw: ");
                Serial.print(*voltage);
                Serial.println();
                debug_tm = millis();
        }
}

void Voltage::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| voltageRaw: ");
                Serial.print(voltage[SENS_RET_RAW_DATA]);
                Serial.print("| voltageAct: ");
                Serial.print(voltage[SENS_RET_ACT_DATA]);
                Serial.print("| voltageAvg: ");
                Serial.print(voltage[SENS_RET_AVG_DATA]);
                Serial.print("| voltageFiltered: ");
                Serial.print(voltage[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void Voltage::getSensorValue(float* value) {
        *value = *voltage;
}

void Voltage::getSensorValue(int* value) {
}

void Voltage::count() {
}

void Voltage::reset() {
}

float Voltage::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *voltage;
        return voltage[c];
}

void Voltage::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

float Voltage::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float Voltage::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}