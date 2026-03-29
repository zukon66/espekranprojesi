#pragma once

#include <string>

namespace espekran::interfaces {

class Storage {
 public:
  virtual ~Storage() = default;
  virtual bool isReady() const = 0;
  virtual bool readText(const std::string& path, std::string& out) = 0;
};

}  // namespace espekran::interfaces
