#pragma once

#ifndef HC_SR04_H
#define HC_SR04_H

#include "Arduino.h"
#include "abstract-sensor.h"
#include "stdint.h"
#include "NewPing.h"
#include <SimpleKalmanFilter.h>

typedef enum {
        HCSR_RET_RAW_DATA,
        HCSR_RET_FILTERED_DATA,
        HCSR_RET_OFFSET_DATA,
        HCSR_RET_ACT_DATA,
        HCSR_RET_AVG_DATA,
        HCSR_RET_TOTAL_DATA,
} hcsr04_index_t;

class HCsr04 : public AbstractSens {
      private:
        NewPing* sensorInstance;
        SimpleKalmanFilter* kf;
        uint16_t data[HCSR_RET_TOTAL_DATA];
        uint16_t trigPin;
        uint16_t echoPin;
        long maxDistance;

        float movingAverageFilter(float newValue);
        float gaussianFilter(float sensorValue);
      public:
        HCsr04();
        ~HCsr04();
        // override
        void init() override;
        void update() override;
        void debug() override;
        // optional
        uint16_t getData(hcsr04_index_t c);
        void setPins(uint8_t __trigg, uint8_t __echo);
        void setMaxDistance(long __dist);
};

#endif  // HC_SR04_H