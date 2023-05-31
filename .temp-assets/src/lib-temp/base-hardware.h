#pragma once

#ifndef BASE_HARDWARE_H
#define BASE_HARDWARE_H

#include "Arduino.h"
#include "HardwareSerial.h"
#include "SPI.h"
#include "base-sensor.h"

// custom enumerate
typedef enum {
        KP_GAINS,
        KI_GAINS,
        KD_GAINS,
        TD_GAINS,
        SERVO_OIL,
        SERVO_WATER,
        SP_OIL,
        SP_WATER,
        MODE,
        AUTOMATIC_START,
        MANUAL_START,
        RESET,
        RELAY_OIL,
        RELAY_WATER,
} data_index_t;

// custom data
typedef struct {
        float kpGains;
        float kiGains;
        float kdGains;
        float tdGains;
        uint16_t servoOil;
        uint16_t servoWater;
        float setPointOil;
        float setPointWater;
        bool mode;
        bool autoStart;
        bool manualStart;
        bool eReset;
        bool relayOil;
        bool relayWater;
        float feedOil;
        float feedWater;
} ExternData;

class HardwareModule : public BaseSens {
      private:
        ExternData* eData;
        uint8_t len;
        char separator;
        bool read;
        bool isReceive;
        String data;

        String parseStr(String data, char separator[], int index);
      public:
        HardwareModule();
        ~HardwareModule();

        void init() override;
        void update() override;
        void debug() override;
        void calibrate() override;

        void setSeparator(char sep);
        void setData(ExternData* dataPtr);

        float getFloatData(uint8_t index);
        int getIntData(uint8_t index);
        bool getBoolData(uint8_t index);
        String getStringData(uint8_t index);

        void getFloatData(float* val, uint8_t index);
        void getIntData(int* val, uint8_t index);
        void getBoolData(bool* val, uint8_t index);
        void getStringData(String* val, uint8_t index);
};

#endif  // BASE_HARDWARE_H