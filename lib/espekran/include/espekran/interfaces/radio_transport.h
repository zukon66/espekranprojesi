#pragma once

#include "espekran/app/app_types.h"

namespace espekran::interfaces {

class RadioTransport {
 public:
  virtual ~RadioTransport() = default;
  virtual bool pollReceive(app::MessagePacket& packet) = 0;
  virtual bool send(const app::MessagePacket& packet) = 0;
  virtual bool linkState() const = 0;
};

}  // namespace espekran::interfaces
