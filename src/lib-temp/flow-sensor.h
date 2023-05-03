#pragma once

#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include "Arduino.h"
#include "base-sensor.h"
#include "stdint.h"

#include "FlowMeter.h"

#define FLOW_RATE_FILTER_KF 8
#define FLOW_VOLUME_FILTER_KF 8
#define PERIOD 1000

class FLowSensor : public BaseSens {
      private:
        void (*flowIntCallback)(void);
        FlowMeter *Meter;
        float* flowRate;
        float* flowVolume;
        uint8_t sensorPin;
        uint32_t update_tm;
        uint32_t debug_tm;
        bool isCalibrate;

        float getSensorAverage(float sensorValue, int numReadings = 10);
        float lowPassFilter(float input, float output, float alpha);
      public:
        FLowSensor(bool enableCalibrate = false);
        ~FLowSensor();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;

        float getValueFlowRate(sens_ret_index_t c = SENS_RET_ACT_DATA);
        float getValueFlowVolume(sens_ret_index_t c = SENS_RET_ACT_DATA);
        void setPins(uint8_t __pin);
        void setCallback(void (*callback)(void));
        void reset();

        void count();
};

#endif  // FLOW_SENSOR_H