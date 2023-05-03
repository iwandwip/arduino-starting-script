#pragma once

#ifndef HCSR04_H
#define HCSR04_H

#include "Arduino.h"
#include "base-sensor.h"
#include "stdint.h"

#include "NewPing.h"

class Sonar : public BaseSens {
      private:
        NewPing* sonar;
        uint8_t echoPin;
        uint8_t triggPin;
        float* distance;
        uint32_t update_tm;
        uint32_t debug_tm;
        bool isCalibrate;

        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
      public:
        Sonar(bool enableCalibrate = false);
        ~Sonar();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t echo, uint8_t trigg);
};

#endif  // HCSR04_H