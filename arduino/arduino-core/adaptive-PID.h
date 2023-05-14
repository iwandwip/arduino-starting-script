/*
 *  adaptive-PID.h
 *
 *  pid control lib
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef ADAPTIVE_PID_H
#define ADAPTIVE_PID_H

#include "Arduino.h"
#include "stdint.h"
#include "math.h"

class AdaptivePID {
      private:
        float kp_, ki_, kd_, td_;
        float e_, le_, se_, de_;
        float out_min_, out_max_;
        float in_min_, in_max_;
        float out_scale_min_, out_scale_max_;
        float in_scale_min_, in_scale_max_;
        float anti_windup_limit_;
        float prev_input_;
        float filtered_de_;
        bool integral_reset_;
        float u_;
      public:
        void SetConstants(float kp, float ki, float kd, float td);
        void SetOutputRange(float min, float max);
        void SetInputRange(float min, float max);
        void SetOutputScale(float min, float max);
        void SetInputScale(float min, float max);
        void SetAntiWindup(float limit);
        void SetDerivativeFilter(float alpha);
        void SetIntegralReset(bool reset);
        void Calculate(float sp, float av);
        float GetError(void);
        float GetOutput(void);
        float GetDeltaError(void);
        float GetNegOutput(void);
        void Reset(void);
};

#endif  // ADAPTIVE_PID_H