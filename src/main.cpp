#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "app_config.h"
#include "board/arduino_clock.h"
#include "board/esp32_board_support.h"
#include "board/serial_display.h"
#include "espekran/app/app_controller.h"
#include "espekran/interfaces/clock.h"
#include "espekran/mocks/mock_power_manager.h"
#include "espekran/mocks/mock_radio_transport.h"
#include "espekran/mocks/null_input.h"
#include "espekran/mocks/null_storage.h"
#include "espekran/ui/ui_presenter.h"

namespace {

espekran::app::AppController gAppController;
espekran::mocks::MockRadioTransport gMockRadio(AppConfig::kInboxCapacity);
espekran::mocks::MockPowerManager gMockPower;
espekran::mocks::NullInput gNullInput;
espekran::mocks::NullStorage gNullStorage;
board::ArduinoClock gClock;
board::SerialDisplay gDisplay(Serial);
SemaphoreHandle_t gAppMutex = nullptr;
bool gWifiConnected = false;
bool gBluetoothEnabled = false;
bool gWeatherToggle = false;

void uiTask(void*) {
  TickType_t lastWakeTime = xTaskGetTickCount();

  for (;;) {
    espekran::ui::UiFrame frame;

    if (xSemaphoreTake(gAppMutex, portMAX_DELAY) == pdTRUE) {
      espekran::input::InputEvent event;
      while (gNullInput.poll(event)) {
        gAppController.handleInput(event);
      }

      gAppController.updateClockFromUptime(gClock.uptimeMs());
      gAppController.pollRadio(gMockRadio);
      gAppController.advanceDemoScreen();
      frame = espekran::ui::buildUiFrame(gAppController.state());
      xSemaphoreGive(gAppMutex);
    }

    gDisplay.render(frame);
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(AppConfig::kUiTickMs));
  }
}

void radioTask(void*) {
  TickType_t lastWakeTime = xTaskGetTickCount();

  for (;;) {
    if (xSemaphoreTake(gAppMutex, portMAX_DELAY) == pdTRUE) {
      const bool queued = gMockRadio.enqueueDemoMessage();
      if (!queued) {
        Serial.println("[RADIO] Mock radio kuyrugu dolu, mesaj dusuruldu.");
      }
      xSemaphoreGive(gAppMutex);
    }

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(AppConfig::kRadioTickMs));
  }
}

void sensorTask(void*) {
  TickType_t lastWakeTime = xTaskGetTickCount();

  for (;;) {
    if (xSemaphoreTake(gAppMutex, portMAX_DELAY) == pdTRUE) {
      gWifiConnected = !gWifiConnected;
      gBluetoothEnabled = !gBluetoothEnabled;
      gWeatherToggle = !gWeatherToggle;

      gMockPower.advanceSimulation();
      gAppController.setConnectivity(gWifiConnected, gBluetoothEnabled);
      gAppController.setWeather(gWeatherToggle ? "22C Gunesli" : "19C Bulutlu");
      gAppController.applyPowerSnapshot(gMockPower.snapshot());
      xSemaphoreGive(gAppMutex);
    }

    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(AppConfig::kSensorTickMs));
  }
}

}  // namespace

void setup() {
  Serial.begin(AppConfig::kSerialBaudRate);
  delay(300);

  board::configurePins();
  board::printBootBanner(Serial);
  gDisplay.begin();

  gAppMutex = xSemaphoreCreateMutex();
  if (gAppMutex == nullptr) {
    Serial.println("[FATAL] Uygulama mutex'i olusturulamadi.");
    for (;;) {
      delay(1000);
    }
  }

  xTaskCreatePinnedToCore(uiTask, "uiTask", 4096, nullptr, 2, nullptr, 1);
  xTaskCreatePinnedToCore(radioTask, "radioTask", 4096, nullptr, 1, nullptr, 0);
  xTaskCreatePinnedToCore(sensorTask, "sensorTask", 3072, nullptr, 1, nullptr, 1);
}

void loop() {
  (void)gNullStorage;
  vTaskDelay(pdMS_TO_TICKS(1000));
}
