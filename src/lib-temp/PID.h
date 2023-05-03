#pragma once

#ifndef PID_H
#define PID_H

#include "Arduino.h"
#include "stdint.h"
#include "math.h"

struct PID {
      private:
        float kp, ki, kd, td;
        float e, le, se;
        float outMin, outMax;
        float u;
      public:
        void SetConstants(float kp, float ki, float kd, float td);
        void SetOutputRange(float min, float max);
        void Calculate(float sp, float av);
        float GetError(void);
        float GetOutput(void);
        float GetNegOutput(void);
        void Reset(void);
};

#endif  // PID_H