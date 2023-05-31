#pragma once

#ifndef TEMPLATE_SENSOR_H
#define TEMPLATE_SENSOR_H

#include "Arduino.h"
#include "stdint.h"
#include "NewPing.h"

template<typename cbT>
using CallbackFunc = cbT (*)(cbT);

template<typename cbT, typename cbU>
using CallbackFuncUpdate = void (*)(cbT, cbU);

typedef enum {
        RAW_DATA = 0,
        FILTER_DATA = 1,
        OFFSET_DATA = 2,
        ACT_DATA = 3,
        AVG_DATA = 4,
        NUM_DATA = 5
};

template<typename dtype, typename cPtr = byte*>
class SensorModule {
      private:
        cPtr sensorClass;
        dtype data[NUM_DATA];

      public:
        SensorModule();
        ~SensorModule();

        void init(CallbackFunc<cPtr> callback);
        void update(CallbackFuncUpdate<dtype*, cPtr> callback);
        cPtr classes();
        dtype* value();
};

template<typename T>
T newpingInitCallback(T packet);
template<typename T, typename U>
void newpingHandlerCallback(T packet, U classses);

#endif  // TEMPLATE_SENSOR_H