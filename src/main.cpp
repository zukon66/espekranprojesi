#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/task.h>

#include "app_config.h"

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
  uint8_t batteryPercent = 100;
  String weatherLabel = "Hazirlaniyor";
  String clockLabel = "--:--";
};

struct MessagePacket {
  uint32_t id = 0;
  char sender[16] = "peer";
  char body[64] = "";
};

struct DeviceState {
  AppScreen currentScreen = AppScreen::Home;
  StatusBarState status{};
  uint32_t unreadMessages = 0;
  uint32_t singlePlayerLaunches = 0;
  uint32_t multiPlayerLaunches = 0;
  bool muteEnabled = false;
  uint8_t brightness = 80;
};

namespace {
DeviceState gState;
QueueHandle_t gInboxQueue = nullptr;

const char* screenName(AppScreen screen) {
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

void updateClock(StatusBarState& status) {
  const uint32_t totalSeconds = millis() / 1000;
  const uint32_t minutes = (totalSeconds / 60) % 60;
  const uint32_t hours = (totalSeconds / 3600) % 24;

  char buffer[6];
  snprintf(buffer, sizeof(buffer), "%02lu:%02lu", hours, minutes);
  status.clockLabel = buffer;
}

void rotateHomePreview(DeviceState& state) {
  static uint8_t step = 0;
  step = (step + 1) % 5;

  switch (step) {
    case 0:
      state.currentScreen = AppScreen::Home;
      break;
    case 1:
      state.currentScreen = AppScreen::Messaging;
      break;
    case 2:
      state.currentScreen = AppScreen::Games;
      break;
    case 3:
      state.currentScreen = AppScreen::Tools;
      break;
    case 4:
      state.currentScreen = AppScreen::Settings;
      break;
  }
}

void printStatusBar(const StatusBarState& status) {
  Serial.printf(
      "[STATUS] Saat=%s | WiFi=%s | BT=%s | NRF=%s | Batarya=%%%u | Hava=%s\n",
      status.clockLabel.c_str(),
      status.wifiConnected ? "Acik" : "Kapali",
      status.bluetoothEnabled ? "Acik" : "Kapali",
      status.radioLinked ? "Bagli" : "Bekliyor",
      status.batteryPercent,
      status.weatherLabel.c_str());
}

void printCurrentScreen(const DeviceState& state) {
  Serial.printf("[UI] Ekran=%s | Okunmamis=%lu | Parlaklik=%%%u | Sessiz=%s\n",
                screenName(state.currentScreen), state.unreadMessages,
                state.brightness, state.muteEnabled ? "Evet" : "Hayir");
}

void enqueueMockMessage() {
  static uint32_t nextId = 1;

  MessagePacket packet;
  packet.id = nextId++;
  snprintf(packet.sender, sizeof(packet.sender), "peer-%lu", packet.id);
  snprintf(packet.body, sizeof(packet.body), "Mesh test mesaji %lu", packet.id);

  if (xQueueSend(gInboxQueue, &packet, 0) != pdPASS) {
    Serial.println("[RADIO] Kuyruk dolu, mesaj dusuruldu.");
  }
}

void processIncomingMessages(DeviceState& state) {
  MessagePacket packet;
  while (xQueueReceive(gInboxQueue, &packet, 0) == pdPASS) {
    state.unreadMessages++;
    state.status.radioLinked = true;
    Serial.printf("[MESAJ] #%lu %s: %s\n", packet.id, packet.sender, packet.body);
  }
}

void uiTask(void*) {
  TickType_t lastWakeTime = xTaskGetTickCount();

  for (;;) {
    updateClock(gState.status);
    processIncomingMessages(gState);
    rotateHomePreview(gState);
    printStatusBar(gState.status);
    printCurrentScreen(gState);
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(AppConfig::kUiTickMs));
  }
}

void radioTask(void*) {
  TickType_t lastWakeTime = xTaskGetTickCount();

  for (;;) {
    enqueueMockMessage();
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(AppConfig::kRadioTickMs));
  }
}

void sensorTask(void*) {
  TickType_t lastWakeTime = xTaskGetTickCount();
  bool weatherToggle = false;

  for (;;) {
    gState.status.wifiConnected = !gState.status.wifiConnected;
    gState.status.bluetoothEnabled = !gState.status.bluetoothEnabled;
    gState.status.batteryPercent =
        gState.status.batteryPercent > 15 ? gState.status.batteryPercent - 1 : 100;
    gState.status.weatherLabel = weatherToggle ? "22C Gunesli" : "19C Bulutlu";
    weatherToggle = !weatherToggle;
    vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(AppConfig::kSensorTickMs));
  }
}

void configurePins() {
  pinMode(AppConfig::kDPadAdcPin, INPUT);
  pinMode(static_cast<uint8_t>(AppConfig::kWakeButtonPin), INPUT_PULLUP);
}

void printBootBanner() {
  Serial.println();
  Serial.println("ESPekran firmware baslatiliyor...");
  Serial.printf("Cihaz: %s\n", AppConfig::kDeviceName);
  Serial.printf("Versiyon: %s\n", AppConfig::kFirmwareVersion);
}
}  // namespace

void setup() {
  Serial.begin(AppConfig::kSerialBaudRate);
  delay(300);

  configurePins();
  printBootBanner();

  gInboxQueue = xQueueCreate(AppConfig::kInboxCapacity, sizeof(MessagePacket));
  if (gInboxQueue == nullptr) {
    Serial.println("[FATAL] Mesaj kuyrugu olusturulamadi.");
    for (;;) {
      delay(1000);
    }
  }

  xTaskCreatePinnedToCore(uiTask, "uiTask", 4096, nullptr, 2, nullptr, 1);
  xTaskCreatePinnedToCore(radioTask, "radioTask", 4096, nullptr, 1, nullptr, 0);
  xTaskCreatePinnedToCore(sensorTask, "sensorTask", 3072, nullptr, 1, nullptr, 1);
}

void loop() {
  // Ana dongu bos birakildi; sistem FreeRTOS task'lariyla yuruyor.
  vTaskDelay(pdMS_TO_TICKS(1000));
}
