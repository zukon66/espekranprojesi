#pragma once

#include "espekran/input/input_event.h"

namespace espekran::interfaces {

class Input {
 public:
  virtual ~Input() = default;
  virtual bool poll(input::InputEvent& event) = 0;
};

}  // namespace espekran::interfaces
