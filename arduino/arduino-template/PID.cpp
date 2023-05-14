/*
 *  PID.c
 *
 *  pid control c
 *  Created on: 2023. 4. 3
 */

#include "PID.h"

void PID::SetConstants(float kp, float ki, float kd, float td) {
        this->kp = kp;
        this->ki = ki;
        this->kd = kd;
        this->td = td;
}
void PID::SetOutputRange(float min, float max) {
        outMin = min;
        outMax = max;
}

void PID::Calculate(float sp, float av) {
        e = sp - av;
        de = e - le;

        u = (kp * e) + (ki * se * td) + (kd * de / td);
        if ((u <= outMin && e > 0) || (u >= outMax && e < 0) || (u > outMin && u < outMax)) {
                se += e;
        }
        le = e;

        u = (u < outMin ? outMin : (u > outMax ? outMax : u));
}

float PID::GetError(void) {
        return e;
}

float PID::GetOutput() {
        return u;
}

float PID::GetDeltaError(void) {
        return de;
}

float PID::GetNegOutput(void) {
        return outMax - u;
}

void PID::Reset() {
        e = 0;
        le = 0;
        se = 0;
        u = 0;
}