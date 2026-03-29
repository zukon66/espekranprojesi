#pragma once

#include <cstdint>
#include <string>

#include "espekran/app/app_types.h"
#include "espekran/input/input_event.h"
#include "espekran/interfaces/radio_transport.h"

namespace espekran::app {

class AppController {
 public:
  AppController();

  void updateClockFromUptime(uint32_t uptimeMs);
  void handleInput(const input::InputEvent& event);
  void advanceDemoScreen();
  void pollRadio(interfaces::RadioTransport& radio);
  void ingestMessage(const MessagePacket& packet);
  void applyPowerSnapshot(const PowerSnapshot& snapshot);
  void setConnectivity(bool wifiConnected, bool bluetoothEnabled);
  void setWeather(std::string weatherLabel);

  const DeviceState& state() const;

 private:
  void moveToNextScreen();
  void moveToPreviousScreen();

  DeviceState state_;
  size_t demoScreenIndex_ = 0;
};

}  // namespace espekran::app
