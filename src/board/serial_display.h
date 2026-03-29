#pragma once

#include <Arduino.h>

#include "espekran/interfaces/display.h"

namespace board {

class SerialDisplay : public espekran::interfaces::Display {
 public:
  explicit SerialDisplay(Stream& stream) : stream_(stream) {}

  void begin() override { stream_.println("[DISPLAY] SerialDisplay hazir."); }

  void render(const espekran::ui::UiFrame& frame) override {
    stream_.printf("[STATUS] %s\n", frame.statusLine.c_str());
    stream_.printf("[UI] Ekran=%s | %s\n", frame.currentScreenLabel.c_str(),
                   frame.summaryLine.c_str());

    for (const auto& notification : frame.notifications) {
      stream_.printf("[NOTIFY] %s\n", notification.c_str());
    }
  }

 private:
  Stream& stream_;
};

}  // namespace board
