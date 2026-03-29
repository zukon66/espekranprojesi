#pragma once

#include <cstdint>

namespace espekran::interfaces {

class Clock {
 public:
  virtual ~Clock() = default;
  virtual uint32_t uptimeMs() const = 0;
};

}  // namespace espekran::interfaces
