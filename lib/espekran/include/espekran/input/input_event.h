#pragma once

#include <cstdint>

namespace espekran::input {

enum class InputEventType : uint8_t {
  None,
  Up,
  Down,
  Left,
  Right,
  Select,
  SleepPressed,
  TouchTap,
};

struct InputEvent {
  InputEventType type = InputEventType::None;
  uint16_t touchX = 0;
  uint16_t touchY = 0;
};

}  // namespace espekran::input
