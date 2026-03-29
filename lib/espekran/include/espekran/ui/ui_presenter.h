#pragma once

#include "espekran/app/app_types.h"
#include "espekran/ui/ui_frame.h"

namespace espekran::ui {

UiFrame buildUiFrame(const app::DeviceState& state);

}  // namespace espekran::ui
