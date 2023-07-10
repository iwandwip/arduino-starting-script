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
#include "Firebase_ESP_Client.h"
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include "stdint.h"

#define API_KEY "AIzaSyAdhnefi4gfQEWSfkzqVUwMZdC8E98oNPI"
#define DATABASE_URL "https://smoke-detector-58544-default-rtdb.firebaseio.com/"

#define USER_EMAIL "smoke-detector@gmail.com"
#define USER_PASSWORD "smoke-detector"

#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600
#define GMT_OFFSET_WIB GMT_OFFSET_SEC * 7
#define DAYLIGHT_OFFSET 3600

void serverHandler(void* pvParameter);

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

        uint8_t connectTime = 0;
        uint32_t sendTime;
        uint32_t getTime;
        bool connect;
        bool mode;

      public:
        FirebaseModule(uint8_t coreIndex = 1);
        ~FirebaseModule();
        bool init();
        bool connectToWiFi(const char* ssid, const char* pwd);
        bool isConnect();
        bool update(void (*onUpdate)(void) = nullptr);
        void clearData();
        void addData(float newData, const char* newAddressData);

        void sendData(uint32_t __time = 2000);
        void setFloat(float floatData, const char* addrs);
        void setString(String strData, const char* addrs);
        float getData(const char* getAddress);
        String getStrData(const char* getAddress);
        void waitConnection(uint32_t __tmr);
};

#endif  // FIREBASE_HANDLER_H