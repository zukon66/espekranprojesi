#pragma once

#include <deque>
#include <string>
#include <utility>
#include <vector>

#include "espekran/interfaces/radio_transport.h"

namespace espekran::mocks {

class MockRadioTransport : public interfaces::RadioTransport {
 public:
  explicit MockRadioTransport(size_t capacity) : capacity_(capacity) {}

  bool pollReceive(app::MessagePacket& packet) override {
    if (inbox_.empty()) {
      return false;
    }

    packet = inbox_.front();
    inbox_.pop_front();
    linked_ = true;
    return true;
  }

  bool send(const app::MessagePacket& packet) override {
    outbox_.push_back(packet);
    linked_ = true;
    return true;
  }

  bool linkState() const override { return linked_; }

  bool enqueueReceived(const app::MessagePacket& packet) {
    if (inbox_.size() >= capacity_) {
      ++droppedCount_;
      return false;
    }

    inbox_.push_back(packet);
    linked_ = true;
    return true;
  }

  bool enqueueDemoMessage() {
    app::MessagePacket packet;
    packet.id = nextId_++;
    packet.sender = "peer-" + std::to_string(packet.id);
    packet.body = "Mock mesh mesaji " + std::to_string(packet.id);
    return enqueueReceived(packet);
  }

  size_t queuedCount() const { return inbox_.size(); }
  size_t droppedCount() const { return droppedCount_; }
  const std::vector<app::MessagePacket>& outbox() const { return outbox_; }

 private:
  std::deque<app::MessagePacket> inbox_;
  std::vector<app::MessagePacket> outbox_;
  size_t capacity_ = 0;
  size_t droppedCount_ = 0;
  uint32_t nextId_ = 1;
  bool linked_ = false;
};

}  // namespace espekran::mocks
