/*

____ ____ _ _  _ ____ ____ _  _     ____ ____ _    _      intrusive_ptr for C++ 20
|    |__/ | |\/| [__  |  | |\ |     |    |___ |    |      version: 1.0.0
|___ |  \ | |  | ___] |__| | \| ___ |___ |___ |___ |___   https://github.com/Patrix9999/intrusive_ptr
                                                                                        
MIT License

Copyright (c) 2026 Patrix9999

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <compare>
#include <type_traits>
#include <utility>

// declaration

namespace crimson_cell {

template <class T>
class intrusive_ptr {
 public:
  using element_type = T;

  constexpr intrusive_ptr() noexcept = default;
  intrusive_ptr(T* p, bool add_ref = true);

  intrusive_ptr(const intrusive_ptr& other);
  intrusive_ptr(intrusive_ptr&& other) noexcept;

  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr(const intrusive_ptr<U>& other);

  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr(intrusive_ptr<U>&& other) noexcept;

  ~intrusive_ptr();

  intrusive_ptr& operator=(const intrusive_ptr& other);
  intrusive_ptr& operator=(intrusive_ptr&& other) noexcept;

  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr& operator=(const intrusive_ptr<U>& other);

  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr& operator=(intrusive_ptr<U>&& other) noexcept;

  void reset();
  void reset(T* p, bool add_ref = true);

  [[nodiscard]] T& operator*() const noexcept;
  [[nodiscard]] T* operator->() const noexcept;
  [[nodiscard]] T* get() const noexcept;
  [[nodiscard]] T* detach() noexcept;

  [[nodiscard]] explicit operator bool() const noexcept;

  void swap(intrusive_ptr& other) noexcept;

 private:
  T* ptr_ = nullptr;
};

// definition

template <class T>
intrusive_ptr<T>::intrusive_ptr(T* p, bool add_ref) : ptr_(p) {
  if (ptr_ && add_ref) intrusive_ptr_add_ref(ptr_);
}

template <class T>
intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr& other) : ptr_(other.ptr_) {
  if (ptr_) intrusive_ptr_add_ref(ptr_);
}

template <class T>
intrusive_ptr<T>::intrusive_ptr(intrusive_ptr&& other) noexcept
    : ptr_(other.ptr_) {
  other.ptr_ = nullptr;
}

template <class T>
template <class U>
  requires std::is_convertible_v<U*, T*>
intrusive_ptr<T>::intrusive_ptr(const intrusive_ptr<U>& other)
    : ptr_(other.get()) {
  if (ptr_) intrusive_ptr_add_ref(ptr_);
}

template <class T>
template <class U>
  requires std::is_convertible_v<U*, T*>
intrusive_ptr<T>::intrusive_ptr(intrusive_ptr<U>&& other) noexcept
    : ptr_(other.detach()) {}

template <class T>
intrusive_ptr<T>::~intrusive_ptr() {
  if (ptr_) intrusive_ptr_release(ptr_);
}

template <class T>
intrusive_ptr<T>& intrusive_ptr<T>::operator=(const intrusive_ptr& other) {
  intrusive_ptr(other).swap(*this);
  return *this;
}

template <class T>
intrusive_ptr<T>& intrusive_ptr<T>::operator=(intrusive_ptr&& other) noexcept {
  intrusive_ptr(std::move(other)).swap(*this);
  return *this;
}
template <class T>
template <class U>
  requires std::is_convertible_v<U*, T*>
intrusive_ptr<T>& intrusive_ptr<T>::operator=(const intrusive_ptr<U>& other) {
  intrusive_ptr(other).swap(*this);
  return *this;
}

template <class T>
template <class U>
  requires std::is_convertible_v<U*, T*>
intrusive_ptr<T>& intrusive_ptr<T>::operator=(
    intrusive_ptr<U>&& other) noexcept {
  intrusive_ptr(std::move(other)).swap(*this);
  return *this;
}

template <class T>
void intrusive_ptr<T>::reset() {
  intrusive_ptr().swap(*this);
}

template <class T>
void intrusive_ptr<T>::reset(T* p, bool add_ref) {
  intrusive_ptr(p, add_ref).swap(*this);
}

template <class T>
T* intrusive_ptr<T>::get() const noexcept {
  return ptr_;
}

template <class T>
T* intrusive_ptr<T>::detach() noexcept {
  T* result = ptr_;
  ptr_ = nullptr;
  return result;
}

template <class T>
T& intrusive_ptr<T>::operator*() const noexcept {
  return *ptr_;
}

template <class T>
T* intrusive_ptr<T>::operator->() const noexcept {
  return ptr_;
}

template <class T>
intrusive_ptr<T>::operator bool() const noexcept {
  return ptr_ != nullptr;
}

template <class T>
void intrusive_ptr<T>::swap(intrusive_ptr& other) noexcept {
  std::swap(ptr_, other.ptr_);
}

// global utilities

template <class T>
constexpr bool operator==(const intrusive_ptr<T>& lhs,
                          const std::nullptr_t rhs) noexcept {
  return lhs.get() == rhs;
}

template <class T, class U>
constexpr bool operator==(const intrusive_ptr<T>& lhs,
                          const intrusive_ptr<U>& rhs) noexcept {
  return lhs.get() == rhs.get();
}

template <class T, class U>
constexpr std::strong_ordering operator<=>(
    const intrusive_ptr<T>& lhs, const intrusive_ptr<U>& rhs) noexcept {
  return lhs.get() <=> rhs.get();
}

template <class T, class U>
intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const& p) {
  return static_cast<T*>(p.get());
}

template <class T, class U>
intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const& p) {
  return const_cast<T*>(p.get());
}

template <class T, class U>
intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const& p) {
  return dynamic_cast<T*>(p.get());
}

template <class T, class U>
intrusive_ptr<T> reinterpret_pointer_cast(const intrusive_ptr<U>& p) {
  return reinterpret_cast<T*>(p.get());
}

}  // namespace crimson_cell

template <typename T>
struct std::hash<crimson_cell::intrusive_ptr<T>> {
  size_t operator()(const crimson_cell::intrusive_ptr<T>& ptr) const noexcept {
    return std::hash<T*>{}(ptr.get());
  }
};