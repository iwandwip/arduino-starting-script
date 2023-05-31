#pragma once

#ifndef BASE_OUTPUT_H
#define BASE_OUTPUT_H

#include "Arduino.h"

class DigitalOut {
      private:
        int _pin;
        unsigned long _delayTime;
        unsigned long _delayStart;
        bool _delayState;
        bool state;
      public:
        DigitalOut();
        DigitalOut(int pin);
        void init();
        void on();
        void off();
        void set(bool state);
        void toggle();
        void onDelay(unsigned long delay_time);
        void offDelay(unsigned long delay_time);
        void update();
        void setPins(uint8_t __pin);
        bool getState();
};

#endif
