#include "flow-sensor.h"

float FLowSensor::getSensorAverage(float sensorValue, int numReadings = 10) {
        float sensorSum = 0;
        for (int i = 0; i < numReadings; i++) {
                sensorSum += sensorValue;
        }
        return sensorSum / (float)numReadings;
}

float FLowSensor::lowPassFilter(float input, float output, float alpha) {
        output = (alpha * input) + ((1.0 - alpha) * output);
        return output;
}

FLowSensor::FLowSensor(bool enableCalibrate = false)
  : sensorPin(2) {
        isCalibrate = enableCalibrate;
        flowRate = (enableCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
        flowVolume = (enableCalibrate) ? new float[SENS_RET_TOTAL_DATA] : new float;
}

FLowSensor::~FLowSensor() {
}


void FLowSensor::init() {
        Meter = new FlowMeter(digitalPinToInterrupt(sensorPin), UncalibratedSensor, flowIntCallback, RISING);
}

void FLowSensor::update() {
        if (millis() - update_tm >= PERIOD) {
                Meter->tick(PERIOD);

                if (!isCalibrate) {
                        *flowRate = Meter->getCurrentFlowrate();
                        *flowRate = *flowRate + (*flowRate * FLOW_RATE_FILTER_KF);
                        *flowRate /= FLOW_RATE_FILTER_KF + 1;

                        *flowVolume = Meter->getTotalVolume();
                        *flowVolume = *flowVolume + (*flowVolume * FLOW_VOLUME_FILTER_KF);
                        *flowVolume /= FLOW_VOLUME_FILTER_KF + 1;
                } else {
                        flowRate[SENS_RET_RAW_DATA] = Meter->getCurrentFlowrate();
                        flowRate[SENS_RET_ACT_DATA] = flowRate[SENS_RET_RAW_DATA] + (flowRate[SENS_RET_RAW_DATA] * FLOW_RATE_FILTER_KF);
                        flowRate[SENS_RET_ACT_DATA] /= FLOW_RATE_FILTER_KF + 1;

                        flowVolume[SENS_RET_RAW_DATA] = Meter->getTotalVolume();
                        flowVolume[SENS_RET_ACT_DATA] = flowVolume[SENS_RET_RAW_DATA] + (flowVolume[SENS_RET_RAW_DATA] * FLOW_VOLUME_FILTER_KF);
                        flowVolume[SENS_RET_ACT_DATA] /= FLOW_VOLUME_FILTER_KF + 1;
                }
                update_tm = millis();
        }
}

void FLowSensor::debug() {
        if (millis() - debug_tm >= 500) {
                if (isCalibrate) return;
                Serial.print("| flowRateRaw: ");
                Serial.print(*flowRate);
                Serial.print("| volumeRaw: ");
                Serial.print(*flowVolume);
                Serial.println();
                debug_tm = millis();
        }
}

void FLowSensor::calibrate() {
        if (millis() - debug_tm >= 500) {
                if (!isCalibrate) return;
                Serial.print("| flowRateRaw: ");
                Serial.print(flowRate[SENS_RET_RAW_DATA]);
                Serial.print("| flowRateAct: ");
                Serial.print(flowRate[SENS_RET_ACT_DATA]);
                Serial.print("| flowRateAvg: ");
                Serial.print(flowRate[SENS_RET_AVG_DATA]);
                Serial.print("| flowRateFiltered: ");
                Serial.print(flowRate[SENS_RET_FILTERED_DATA]);
                Serial.print("| volumeRaw: ");
                Serial.print(flowVolume[SENS_RET_RAW_DATA]);
                Serial.print("| volumeAct: ");
                Serial.print(flowVolume[SENS_RET_ACT_DATA]);
                Serial.print("| volumeAvg: ");
                Serial.print(flowVolume[SENS_RET_AVG_DATA]);
                Serial.print("| volumeFiltered: ");
                Serial.print(flowVolume[SENS_RET_FILTERED_DATA]);
                Serial.println();
                debug_tm = millis();
        }
}

float FLowSensor::getValueFlowRate(sens_ret_index_t c) {
        if (!isCalibrate) return *flowRate;
        return flowRate[c];
}

float FLowSensor::getValueFlowVolume(sens_ret_index_t c) {
        if (!isCalibrate) return *flowVolume;
        return flowVolume[c];
}

void FLowSensor::setPins(uint8_t __pin) {
        this->sensorPin = __pin;
}

void FLowSensor::setCallback(void (*callback)(void)) {
        flowIntCallback = callback;
}

void FLowSensor::reset() {
        flowRate[SENS_RET_RAW_DATA] = 0.0;
        flowVolume[SENS_RET_RAW_DATA] = 0.0;
}

void FLowSensor::count() {
        Meter->count();
}