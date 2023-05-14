/*
 *  firebase_handler.h
 *
 *  firebase handler module
 *  Created on: 2023. 4. 3
 */

#pragma once

#ifndef FIREBASE_HANDLER_H
#define FIREBASE_HANDLER_H

#include "Arduino.h"
#include "stdint.h"

#if defined(ESP32)
#include <WiFi.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include "ets_sys.h"
#include "osapi.h"
#include "os_type.h"
#endif

#include "Firebase_ESP_Client.h"

#define API_KEY "AIzaSyB5_cRkAsznDYmkAYnWaqdozHYbkBz8iRI"
#define DATABASE_URL "https://infusion-checker-default-rtdb.firebaseio.com/"

#define USER_EMAIL "infusion-checker@gmail.com"
#define USER_PASSWORD "infusion-checker"

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600
#define GMT_OFFSET_WIB GMT_OFFSET_SEC * 7
#define DAYLIGHT_OFFSET 3600

#if defined(ESP32)
void serverHandler(void* pvParameter);
#elif defined(ESP8266)
void serverHandler(os_event_t* events);
#endif

class FirebaseModule {
      private:
#if defined(ESP32)
        TaskHandle_t* serverTask = nullptr;
#endif
        FirebaseData* fbdo = nullptr;
        FirebaseAuth* auth = nullptr;
        FirebaseConfig* config = nullptr;

        float* data = nullptr;
        char** address = nullptr;
        uint8_t dataLen = 0;
        uint8_t addrLen = 0;

        uint32_t sendTime;
        uint32_t getTime;
        bool connect;
      public:
        FirebaseModule(uint8_t coreIndex = 1);
        ~FirebaseModule();
        bool init();
        bool connectToWiFi(const char* ssid, const char* pwd);
        bool isConnect();
        void update(void (*onUpdate)(void) = nullptr);
        void addData(float newData, const char* newAddressData);
        void sendData(uint32_t __time = 2000);
        void clearData();
        void setFloat(float floatData, const char* addrs);
        void setString(String strData, const char* addrs);
        float getData(const char* getAddress);
        String getStrData(const char* getAddress);
        void waitConnection(uint32_t __tmr);
};

#endif  // FIREBASE_HANDLER_H