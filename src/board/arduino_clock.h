#pragma once

#include <Arduino.h>

#include "espekran/interfaces/clock.h"

namespace board {

class ArduinoClock : public espekran::interfaces::Clock {
 public:
  uint32_t uptimeMs() const override { return millis(); }
};

}  // namespace board
