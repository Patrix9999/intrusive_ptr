#pragma once

#include <cassert>

class RefCountedObject {
 public:
  RefCountedObject() noexcept = default;

  void AddRef() const noexcept { ++ref_count_; }

  void Release() const noexcept {
    assert(ref_count_ > 0);

    --ref_count_;
    if (ref_count_ == 0) delete this;
  }

  [[nodiscard]] constexpr int RefCount() const noexcept { return ref_count_; }

 protected:
  virtual ~RefCountedObject() noexcept = default;

 private:
  mutable int ref_count_ = 0;
};

inline void intrusive_ptr_add_ref(const RefCountedObject* object) noexcept {
  object->AddRef();
}

inline void intrusive_ptr_release(const RefCountedObject* object) noexcept {
  object->Release();
}