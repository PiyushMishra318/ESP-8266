#pragma once

#ifndef WIFI_SSID
#include "secrets.h"
#endif

#ifndef WIFI_SSID
#error "Copy secrets.example.h to secrets.h and set your WiFi credentials"
#endif

constexpr char kPushingBoxHost[] = "api.pushingbox.com";
constexpr uint16_t kPushingBoxPort = 80;
constexpr uint32_t kSampleIntervalMs = 10000;
constexpr uint8_t kDhtPin = D4;
