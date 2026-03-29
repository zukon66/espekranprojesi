#pragma once

#include "espekran/interfaces/storage.h"

namespace espekran::mocks {

class NullStorage : public interfaces::Storage {
 public:
  bool isReady() const override { return false; }

  bool readText(const std::string& path, std::string& out) override {
    (void)path;
    out.clear();
    return false;
  }
};

}  // namespace espekran::mocks
