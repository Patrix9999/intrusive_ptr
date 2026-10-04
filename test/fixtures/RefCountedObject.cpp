#include "RefCountedObject.hpp"

#include <cassert>

void RefCountedObject::AddRef() noexcept { ++ref_count_; }

void RefCountedObject::Release() noexcept {
  assert(ref_count_ > 0);

  --ref_count_;
  if (ref_count_ == 0) delete this;
}
