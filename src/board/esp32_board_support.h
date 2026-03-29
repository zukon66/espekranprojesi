#pragma once

#include <Arduino.h>

#include "app_config.h"

namespace board {

inline void configurePins() {
  pinMode(AppConfig::kDPadAdcPin, INPUT);
  pinMode(static_cast<uint8_t>(AppConfig::kWakeButtonPin), INPUT_PULLUP);
}

inline void printBootBanner(Stream& stream) {
  stream.println();
  stream.println("ESPekran firmware baslatiliyor...");
  stream.printf("Cihaz: %s\n", AppConfig::kDeviceName);
  stream.printf("Versiyon: %s\n", AppConfig::kFirmwareVersion);
  stream.println("Durum: Katmanli yazilim simulesi, donanim dogrulamasi yapilmadi.");
}

}  // namespace board
