#include "RefCountedObject.hpp"

#include <cassert>

void RefCountedObject::AddRef() noexcept { ++ref_count_; }

void RefCountedObject::Release() noexcept {
  assert(ref_count_ > 0);

  --ref_count_;
  if (ref_count_ == 0) delete this;
}

void intrusive_ptr_add_ref(RefCountedObject* object) noexcept {
  object->AddRef();
}

void intrusive_ptr_release(RefCountedObject* object) noexcept {
  object->Release();
}