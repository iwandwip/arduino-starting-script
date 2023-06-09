/*
 *  esp-cam.h
 *
 *  esp camera module
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef ESP_CAM_H
#define ESP_CAM_H

#include "Arduino.h"
#include "WiFi.h"
#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include "config.h"
#include "SPIFFS.h"
#include "FS.h"
#include "esp-cam-config.h"

class EspCamera {
      private:
        String imgStorePath;
        bool check(fs::FS &fs);
      public:
        EspCamera();
        ~EspCamera();
        bool init();
        void capture();
        void setPath(String _path);
        String getPath();

        void setFrameSize(int _frame_size);
        void setJpegQuality(int _size);
        void setFbCount(int _size);

        void setConfig(camera_config_t _config);
        void setMinPicSize(int _size);
};

#endif  // ESP_CAM_H
