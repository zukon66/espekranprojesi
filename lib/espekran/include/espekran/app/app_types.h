#pragma once

#include <cstdint>
#include <string>

namespace espekran::app {

enum class AppScreen : uint8_t {
  Home,
  Messaging,
  Games,
  Files,
  Tools,
  Settings,
};

struct StatusBarState {
  bool wifiConnected = false;
  bool bluetoothEnabled = false;
  bool radioLinked = false;
  bool charging = false;
  bool lowPowerMode = false;
  uint8_t batteryPercent = 100;
  std::string weatherLabel = "Hazirlaniyor";
  std::string clockLabel = "--:--";
};

struct MessagePacket {
  uint32_t id = 0;
  std::string sender = "peer";
  std::string body;
};

struct PowerSnapshot {
  uint8_t batteryPercent = 100;
  bool charging = false;
  bool lowPowerMode = false;
};

struct DeviceState {
  AppScreen currentScreen = AppScreen::Home;
  StatusBarState status{};
  uint32_t unreadMessages = 0;
  uint32_t singlePlayerLaunches = 0;
  uint32_t multiPlayerLaunches = 0;
  bool muteEnabled = false;
  bool sleepRequested = false;
  uint8_t brightness = 80;
  std::string lastMessagePreview;
};

const char* toString(AppScreen screen);

}  // namespace espekran::app
