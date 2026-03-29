#pragma once

#include "espekran/interfaces/power_manager.h"

namespace espekran::mocks {

class MockPowerManager : public interfaces::PowerManager {
 public:
  app::PowerSnapshot snapshot() const override { return snapshot_; }

  void requestSleep() override { sleepRequested_ = true; }

  void advanceSimulation() {
    snapshot_.batteryPercent =
        snapshot_.batteryPercent > 15 ? static_cast<uint8_t>(snapshot_.batteryPercent - 1)
                                      : static_cast<uint8_t>(100);
    snapshot_.charging = !snapshot_.charging;
    snapshot_.lowPowerMode = snapshot_.batteryPercent < 25;
  }

  bool sleepRequested() const { return sleepRequested_; }

 private:
  app::PowerSnapshot snapshot_{};
  bool sleepRequested_ = false;
};

}  // namespace espekran::mocks
