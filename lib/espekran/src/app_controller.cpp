#include "espekran/app/app_controller.h"

#include <array>
#include <cstdio>
#include <utility>

namespace espekran::app {

namespace {

constexpr std::array<AppScreen, 6> kDemoScreens = {
    AppScreen::Home, AppScreen::Messaging, AppScreen::Games,
    AppScreen::Files, AppScreen::Tools, AppScreen::Settings};

std::string formatClock(uint32_t uptimeMs) {
  const uint32_t totalSeconds = uptimeMs / 1000;
  const uint32_t minutes = (totalSeconds / 60) % 60;
  const uint32_t hours = (totalSeconds / 3600) % 24;

  char buffer[6];
  std::snprintf(buffer, sizeof(buffer), "%02lu:%02lu",
                static_cast<unsigned long>(hours),
                static_cast<unsigned long>(minutes));
  return buffer;
}

}  // namespace

const char* toString(AppScreen screen) {
  switch (screen) {
    case AppScreen::Home:
      return "Ana Ekran";
    case AppScreen::Messaging:
      return "Mesajlasma";
    case AppScreen::Games:
      return "Oyunlar";
    case AppScreen::Files:
      return "Dosya Yoneticisi";
    case AppScreen::Tools:
      return "Araclar";
    case AppScreen::Settings:
      return "Ayarlar";
  }
  return "Bilinmeyen";
}

AppController::AppController() = default;

void AppController::updateClockFromUptime(uint32_t uptimeMs) {
  state_.status.clockLabel = formatClock(uptimeMs);
}

void AppController::handleInput(const input::InputEvent& event) {
  switch (event.type) {
    case input::InputEventType::Up:
    case input::InputEventType::Left:
      moveToPreviousScreen();
      break;
    case input::InputEventType::Down:
    case input::InputEventType::Right:
      moveToNextScreen();
      break;
    case input::InputEventType::Select:
      if (state_.currentScreen == AppScreen::Messaging) {
        state_.unreadMessages = 0;
      }
      break;
    case input::InputEventType::SleepPressed:
      state_.sleepRequested = true;
      break;
    case input::InputEventType::TouchTap:
    case input::InputEventType::None:
      break;
  }
}

void AppController::advanceDemoScreen() { moveToNextScreen(); }

void AppController::pollRadio(interfaces::RadioTransport& radio) {
  MessagePacket packet;
  while (radio.pollReceive(packet)) {
    ingestMessage(packet);
  }
  state_.status.radioLinked = radio.linkState();
}

void AppController::ingestMessage(const MessagePacket& packet) {
  ++state_.unreadMessages;
  state_.status.radioLinked = true;
  state_.lastMessagePreview = packet.sender + ": " + packet.body;
}

void AppController::applyPowerSnapshot(const PowerSnapshot& snapshot) {
  state_.status.batteryPercent = snapshot.batteryPercent;
  state_.status.charging = snapshot.charging;
  state_.status.lowPowerMode = snapshot.lowPowerMode;
}

void AppController::setConnectivity(bool wifiConnected, bool bluetoothEnabled) {
  state_.status.wifiConnected = wifiConnected;
  state_.status.bluetoothEnabled = bluetoothEnabled;
}

void AppController::setWeather(std::string weatherLabel) {
  state_.status.weatherLabel = std::move(weatherLabel);
}

const DeviceState& AppController::state() const { return state_; }

void AppController::moveToNextScreen() {
  demoScreenIndex_ = (demoScreenIndex_ + 1) % kDemoScreens.size();
  state_.currentScreen = kDemoScreens[demoScreenIndex_];
}

void AppController::moveToPreviousScreen() {
  if (demoScreenIndex_ == 0) {
    demoScreenIndex_ = kDemoScreens.size() - 1;
  } else {
    --demoScreenIndex_;
  }
  state_.currentScreen = kDemoScreens[demoScreenIndex_];
}

}  // namespace espekran::app
