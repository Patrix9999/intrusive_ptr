#pragma once

class RefCountedObject {
 public:
  RefCountedObject() noexcept = default;
  ~RefCountedObject() noexcept = default;

  void AddRef() noexcept;
  void Release() noexcept;

  [[nodiscard]] constexpr int RefCount() const noexcept { return ref_count_; }

 private:
  int ref_count_ = 0;
};

void intrusive_ptr_add_ref(RefCountedObject* object) noexcept;
void intrusive_ptr_release(RefCountedObject* object) noexcept;