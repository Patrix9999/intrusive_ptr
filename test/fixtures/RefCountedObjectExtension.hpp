#pragma once

#include "RefCountedObject.hpp"

class RefCountedObjectExtension : public RefCountedObject {
public:
  RefCountedObjectExtension() noexcept = default;
  ~RefCountedObjectExtension() noexcept override = default;

  [[nodiscard]] constexpr int Value() const noexcept { return dummy_member_; }

private:
  int dummy_member_ = 1337;
};