/*
 *  adaptive-.cpp
 *
 *  pid control c
 *  Created on: 2023. 4. 3
 */

#include "adaptive-PID.h"

void AdaptivePID::SetConstants(float kp, float ki, float kd, float td) {
        kp_ = kp;
        ki_ = ki;
        kd_ = kd;
        td_ = td;
}

void AdaptivePID::SetOutputRange(float min, float max) {
        out_min_ = min;
        out_max_ = max;
}

void AdaptivePID::SetInputRange(float min, float max) {
        in_min_ = min;
        in_max_ = max;
}

void AdaptivePID::SetOutputScale(float min, float max) {
        out_scale_min_ = min;
        out_scale_max_ = max;
}

void AdaptivePID::SetInputScale(float min, float max) {
        in_scale_min_ = min;
        in_scale_max_ = max;
}

void AdaptivePID::SetAntiWindup(float limit) {
        anti_windup_limit_ = limit;
}

void AdaptivePID::SetDerivativeFilter(float alpha) {
        filtered_de_ = alpha * filtered_de_ + (1 - alpha) * de_;
}

void AdaptivePID::SetIntegralReset(bool reset) {
        integral_reset_ = reset;
}

void AdaptivePID::Calculate(float sp, float av) {
        // Scale input values
        float sp_scaled = (sp - in_min_) / (in_max_ - in_min_) * (in_scale_max_ - in_scale_min_) + in_scale_min_;
        float av_scaled = (av - in_min_) / (in_max_ - in_min_) * (in_scale_max_ - in_scale_min_) + in_scale_min_;
        e_ = sp_scaled - av_scaled;
        de_ = (e_ - le_) * (in_scale_max_ - in_scale_min_) / (td_ * (in_max_ - in_min_));

        // Filter derivative term to remove high-frequency noise
        SetDerivativeFilter(0.5);
        float u = (kp_ * e_) + (ki_ * se_ * td_) + (kd_ * filtered_de_ / td_);

        // Check output limits and apply anti-windup
        if (u > out_max_) {
                u_ = out_max_;
                if (integral_reset_) {
                        se_ = 0;
                }
        } else if (u < out_min_) {
                u_ = out_min_;
                if (integral_reset_) {
                        se_ = 0;
                }
        } else {
                u_ = u;
                se_ += e_;
        }

        // Apply anti-windup limit
        if (se_ > anti_windup_limit_) {
                se_ = anti_windup_limit_;
        } else if (se_ < -anti_windup_limit_) {
                se_ = -anti_windup_limit_;
        }

        // Scale output value
        float output_scaled = (u_ - out_min_) / (out_max_ - out_min_) * (out_scale_max_ - out_scale_min_) + out_scale_min_;
}

float AdaptivePID::GetError(void) {
        return e_;
}

float AdaptivePID::GetOutput(void) {
        return u_;
}

float AdaptivePID::GetDeltaError(void) {
        return de_;
}

float AdaptivePID::GetNegOutput(void) {
        return -u_;
}

void AdaptivePID::Reset(void) {
        le_ = 0;
        se_ = 0;
        de_ = 0;
        e_ = 0;
        u_ = 0;
        prev_input_ = 0;
        filtered_de_ = 0;
}