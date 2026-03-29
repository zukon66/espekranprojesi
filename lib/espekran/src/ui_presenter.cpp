#include "espekran/ui/ui_presenter.h"

#include <sstream>

namespace espekran::ui {

UiFrame buildUiFrame(const app::DeviceState& state) {
  UiFrame frame;

  std::ostringstream status;
  status << "Saat=" << state.status.clockLabel << " | WiFi="
         << (state.status.wifiConnected ? "Acik" : "Kapali") << " | BT="
         << (state.status.bluetoothEnabled ? "Acik" : "Kapali") << " | Radio="
         << (state.status.radioLinked ? "Bagli" : "Bekliyor") << " | Batarya=%"
         << static_cast<int>(state.status.batteryPercent) << " | Hava="
         << state.status.weatherLabel;
  if (state.status.lowPowerMode) {
    status << " | DusukGuc";
  }
  if (state.status.charging) {
    status << " | Sarj";
  }

  std::ostringstream summary;
  summary << "Okunmamis=" << state.unreadMessages << " | Parlaklik=%"
          << static_cast<int>(state.brightness) << " | Sessiz="
          << (state.muteEnabled ? "Evet" : "Hayir");

  frame.statusLine = status.str();
  frame.summaryLine = summary.str();
  frame.currentScreenLabel = app::toString(state.currentScreen);

  if (!state.lastMessagePreview.empty()) {
    frame.notifications.push_back(state.lastMessagePreview);
  }

  if (state.sleepRequested) {
    frame.notifications.push_back("Uyku istegi alindi");
  }

  return frame;
}

}  // namespace espekran::ui
