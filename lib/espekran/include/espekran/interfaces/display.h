#pragma once

#include "espekran/ui/ui_frame.h"

namespace espekran::interfaces {

class Display {
 public:
  virtual ~Display() = default;
  virtual void begin() = 0;
  virtual void render(const ui::UiFrame& frame) = 0;
};

}  // namespace espekran::interfaces
