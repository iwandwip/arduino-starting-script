#include "hc-sr04.h"

#define ULTRA_MAX_DISTANCE 200
#define ULTRA_SAMPLING_TIME 300
#define ULTRA_FILTER_KF 8

#define WINDOW_SIZE 5
#define NO_OFFSET 0

float HCsr04::movingAverageFilter(float newValue) {
        static float window[WINDOW_SIZE];
        static int index = 0;
        static float sum = 0;

        sum -= window[index];
        sum += newValue;
        window[index] = newValue;
        index = (index + 1) % WINDOW_SIZE;

        return sum / WINDOW_SIZE;
}

float HCsr04::gaussianFilter(float sensorValue) {
        const float filterWeight[7] = { 0.06136, 0.24477, 0.38774, 0.24477, 0.06136 };
        const int filterLength = 5;

        static float sensorBuffer[5];
        static int index = 0;

        float filteredValue = 0.0;
        sensorBuffer[index] = sensorValue;
        index++;

        if (index >= filterLength) index = 0;
        for (int i = 0; i < filterLength; i++) {
                int bufferIndex = (index + i) % filterLength;
                filteredValue += filterWeight[i] * sensorBuffer[bufferIndex];
        }
        return filteredValue;
}

HCsr04::HCsr04() {
        maxDistance = ULTRA_MAX_DISTANCE;
        kf = new SimpleKalmanFilter(2, 2, 0.01);
}

HCsr04::~HCsr04() {
        delete sensorInstance;
        delete this;
}

void HCsr04::init() {
        sensorInstance = new NewPing(trigPin, echoPin, 200);
}

void HCsr04::update() {
        static uint32_t sens_tmr;
        if (millis() - sens_tmr >= 30) {
                data[HCSR_RET_RAW_DATA] = sensorInstance->ping_cm();
                data[HCSR_RET_ACT_DATA] = data[HCSR_RET_RAW_DATA] + (data[HCSR_RET_RAW_DATA] * ULTRA_FILTER_KF);
                data[HCSR_RET_ACT_DATA] /= ULTRA_FILTER_KF + 1;

                data[HCSR_RET_AVG_DATA] = movingAverageFilter(data[HCSR_RET_ACT_DATA]);
                // data[HCSR_RET_FILTERED_DATA] = gaussianFilter(data[HCSR_RET_ACT_DATA]);
                data[HCSR_RET_FILTERED_DATA] = kf->updateEstimate(data[HCSR_RET_FILTERED_DATA]);
                sens_tmr = millis();
        }
}

void HCsr04::debug() {
}

uint16_t HCsr04::getData(hcsr04_index_t c) {
        return data[c];
}

void HCsr04::setPins(uint8_t __trigg, uint8_t __echo) {
        this->trigPin = __trigg;
        this->echoPin = __echo;
}

void HCsr04::setMaxDistance(long __dist) {
        this->maxDistance = __dist;
}