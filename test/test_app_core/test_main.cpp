#include <unity.h>

#include "espekran/app/app_controller.h"
#include "espekran/mocks/mock_radio_transport.h"

namespace {

void test_ui_state_transitions_are_deterministic() {
  espekran::app::AppController controller;

  TEST_ASSERT_EQUAL_INT(static_cast<int>(espekran::app::AppScreen::Home),
                        static_cast<int>(controller.state().currentScreen));

  controller.advanceDemoScreen();
  TEST_ASSERT_EQUAL_INT(static_cast<int>(espekran::app::AppScreen::Messaging),
                        static_cast<int>(controller.state().currentScreen));

  controller.handleInput({espekran::input::InputEventType::Right, 0, 0});
  TEST_ASSERT_EQUAL_INT(static_cast<int>(espekran::app::AppScreen::Games),
                        static_cast<int>(controller.state().currentScreen));

  controller.handleInput({espekran::input::InputEventType::Left, 0, 0});
  TEST_ASSERT_EQUAL_INT(static_cast<int>(espekran::app::AppScreen::Messaging),
                        static_cast<int>(controller.state().currentScreen));

  TEST_ASSERT_EQUAL_UINT32(0, controller.state().unreadMessages);
}

void test_mock_radio_messages_update_state() {
  espekran::app::AppController controller;
  espekran::mocks::MockRadioTransport radio(2);

  TEST_ASSERT_TRUE(radio.enqueueReceived({1, "peer-a", "selam"}));
  TEST_ASSERT_TRUE(radio.enqueueReceived({2, "peer-b", "durum"}));
  TEST_ASSERT_FALSE(radio.enqueueReceived({3, "peer-c", "tasma"}));
  TEST_ASSERT_EQUAL_UINT32(1, static_cast<uint32_t>(radio.droppedCount()));

  controller.pollRadio(radio);

  TEST_ASSERT_EQUAL_UINT32(2, controller.state().unreadMessages);
  TEST_ASSERT_TRUE(controller.state().status.radioLinked);
  TEST_ASSERT_EQUAL_UINT32(0, static_cast<uint32_t>(radio.queuedCount()));
  TEST_ASSERT_EQUAL_STRING("peer-b: durum",
                           controller.state().lastMessagePreview.c_str());
}

}  // namespace

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_ui_state_transitions_are_deterministic);
  RUN_TEST(test_mock_radio_messages_update_state);
  return UNITY_END();
}
