/*
 *  load-cell.h
 *
 *  load-cell sensor lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef LOADCELL_H
#define LOADCELL_H

#include "Arduino.h"
#include "sensor-module.h"
#include "stdint.h"

#include "HX711.h"

class LoadCell : public BaseSens {
      private:
        HX711* scale;
        uint8_t doutPin;
        uint8_t sckPin;
        float* weight;

        uint32_t update_tm;
        uint32_t debug_tm;
        uint32_t* cal_tm;
        uint32_t sleep_time;
        
        bool isCalibrate;
        bool sleep;

        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
      public:
        LoadCell(bool enableCalibrate = false);
        LoadCell(uint8_t dout, uint8_t sck, bool enableCalibrate = false);
        ~LoadCell();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;
        void getSensorValue(float* value) override;
        void getSensorValue(int* value) override;
        void count() override;
        void reset() override;

        float getValue(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t dout, uint8_t sck);
};

#endif  // LOADCELL_H