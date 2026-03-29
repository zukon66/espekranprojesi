#pragma once

#include <string>
#include <vector>

namespace espekran::ui {

struct UiFrame {
  std::string statusLine;
  std::string summaryLine;
  std::string currentScreenLabel;
  std::vector<std::string> notifications;
};

}  // namespace espekran::ui
