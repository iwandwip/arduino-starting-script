/*
 *  voltage-sens.h
 *
 *  voltage sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef VOLTAGE_SENS_H
#define VOLTAGE_SENS_H

#include "Arduino.h"
#include "sensor-module.h"
#include "stdint.h"

class Voltage : public BaseSens {
      private:
        uint8_t sensorPin;
        float* voltage;
        uint32_t update_tm;
        uint32_t debug_tm;
        uint32_t* cal_tm;
        bool isCalibrate;

        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
      public:
        Voltage();
        Voltage(uint8_t __pin, bool enableCalibrate = false);
        ~Voltage();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;
        void getSensorValue(float* value) override;
        void getSensorValue(int* value) override;
        void count() override;
        void reset() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t __pin);
};

#endif  // VOLTAGE_SENS_H