#pragma once

#include <Arduino.h>

namespace AppConfig {
constexpr uint32_t kSerialBaudRate = 115200;
constexpr uint32_t kUiTickMs = 250;
constexpr uint32_t kRadioTickMs = 500;
constexpr uint32_t kSensorTickMs = 1000;
constexpr uint32_t kInboxCapacity = 10;

constexpr gpio_num_t kWakeButtonPin = GPIO_NUM_33;
constexpr int kDPadAdcPin = 34;

constexpr char kDeviceName[] = "ESPekran-01";
constexpr char kFirmwareVersion[] = "0.2.0";
}  // namespace AppConfig
