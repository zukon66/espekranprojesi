#pragma once

#include "espekran/app/app_types.h"

namespace espekran::interfaces {

class PowerManager {
 public:
  virtual ~PowerManager() = default;
  virtual app::PowerSnapshot snapshot() const = 0;
  virtual void requestSleep() = 0;
};

}  // namespace espekran::interfaces
