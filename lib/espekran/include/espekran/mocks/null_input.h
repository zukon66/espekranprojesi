#pragma once

#include "espekran/interfaces/input.h"

namespace espekran::mocks {

class NullInput : public interfaces::Input {
 public:
  bool poll(input::InputEvent& event) override {
    event = {};
    return false;
  }
};

}  // namespace espekran::mocks
