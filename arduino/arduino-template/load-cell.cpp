/*
 *  load-cell.h
 *
 *  load-cell sensor c
 *  Created on: 2023. 4. 3
 */

#include "load-cell.h"
#include "SimpleKalmanFilter.h"

#define SCALE_FILTER_KF 8

LoadCell::LoadCell(bool enableCalibrate)
  : doutPin(2), sckPin(3) {
        isCalibrate = enableCalibrate;
        weight = (enableCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (enableCalibrate) cal_tm = new uint32_t;
}

LoadCell::LoadCell(uint8_t dout, uint8_t sck, bool enableCalibrate) {
        this->doutPin = dout;
        this->sckPin = sck;
        isCalibrate = enableCalibrate;
        weight = (isCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        if (isCalibrate) cal_tm = new uint32_t;
}

LoadCell::~LoadCell() {
        delete this;
}

void LoadCell::init() {
        scale = new HX711;
        (*scale).begin(this->doutPin, this->sckPin);
        (*scale).read();
        (*scale).read_average(20);
        (*scale).get_value(5);
        (*scale).get_units(5);
        (*scale).set_scale(2280.f);
        (*scale).tare();
        (*scale).read();
        (*scale).read_average(20);
        (*scale).get_value(5);
        (*scale).get_units(5);
        sleep = false;
}

void LoadCell::update() {
        if (millis() - update_tm >= 500) {
                if (scale->is_ready()) {
                        if (!isCalibrate) {
                                *weight = scale->get_units();
                                *weight = *weight + (*weight * SCALE_FILTER_KF);
                                *weight /= SCALE_FILTER_KF + 1;
                        } else {
                                SimpleKalmanFilter* sonarKf = new SimpleKalmanFilter(2, 2, 0.01);
                                weight[SENS_RET_RAW_DATA] = scale->get_units();
                                weight[SENS_RET_ACT_DATA] = weight[SENS_RET_RAW_DATA] + (weight[SENS_RET_RAW_DATA] * SCALE_FILTER_KF);
                                weight[SENS_RET_ACT_DATA] /= SCALE_FILTER_KF + 1;

                                weight[SENS_RET_AVG_DATA] = getSensorAverage(weight[SENS_RET_ACT_DATA]);
                                weight[SENS_RET_FILTERED_DATA] = lowPassFilter(weight[SENS_RET_ACT_DATA], weight[SENS_RET_FILTERED_DATA], 0.1);
                                weight[SENS_RET_FILTERED_DATA] = sonarKf->updateEstimate(weight[SENS_RET_ACT_DATA]);
                                delete sonarKf;
                        }
                }
                update_tm = millis();
        }
        if (millis() - sleep_time >= 1000) {
                sleep = (sleep) ? false : true;
                if (sleep) scale->power_down();
                else scale->power_up();
                sleep_time = millis();
        }
}

void LoadCell::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| weightRaw: ");
                Serial.print(*weight);
                Serial.println();
                debug_tm = millis();
        }
}

void LoadCell::calibrate() {
        if (millis() - *cal_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| weightRaw: ");
                Serial.print(weight[SENS_RET_RAW_DATA]);
                Serial.print("| weightAct: ");
                Serial.print(weight[SENS_RET_ACT_DATA]);
                Serial.print("| weightAvg: ");
                Serial.print(weight[SENS_RET_AVG_DATA]);
                Serial.print("| weightFiltered: ");
                Serial.print(weight[SENS_RET_FILTERED_DATA]);
                Serial.println();
                *cal_tm = millis();
        }
}

void LoadCell::getSensorValue(float* value) {
        *value = *weight;
}

void LoadCell::getSensorValue(int* value) {
}

void LoadCell::count() {
}

void LoadCell::reset() {
        scale->tare();
}

float LoadCell::getValue(sens_ret_index_t c) {
        if (!isCalibrate) return *weight;
        return weight[c];
}

void LoadCell::setPins(uint8_t dout, uint8_t sck) {
        this->doutPin = dout;
        this->sckPin = sck;
}

float LoadCell::getSensorAverage(float sensorValue, int numReadings) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float LoadCell::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}