// clang-format off
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
// clang-format on

#pragma once

#include <compare>
#include <functional>
#include <type_traits>
#include <utility>

// declaration

namespace crimson_cell {

/**
 * @brief A lightweight intrusive reference-counted smart pointer.
 *
 * @tparam T The managed object type.
 *
 * The managed type is responsible for maintaining its own reference count.
 *
 * The following functions must be available through ADL:
 * - intrusive_ptr_add_ref(T*)
 * - intrusive_ptr_release(T*)
 */
template <class T>
class intrusive_ptr {
 public:
  /// @brief The managed object type.
  using element_type = T;

  /// @brief Constructs an empty intrusive pointer.
  constexpr intrusive_ptr() noexcept = default;

  /// @brief Constructs a pointer from a raw pointer.
  ///
  /// @param p Pointer to the managed object.
  /// @param add_ref Whether to acquire a reference to the object.
  intrusive_ptr(T* p, bool add_ref = true);

  /// @brief Copies an intrusive pointer and acquires a reference.
  ///
  /// @param other Pointer to copy.
  intrusive_ptr(const intrusive_ptr& other);

  /// @brief Moves an intrusive pointer.
  ///
  /// @param other Pointer to move from.
  intrusive_ptr(intrusive_ptr&& other) noexcept;

  /// @brief Converts an intrusive pointer to a compatible type.
  ///
  /// @tparam U Source object type.
  /// @param other Pointer to convert.
  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr(const intrusive_ptr<U>& other);

  /// @brief Moves an intrusive pointer to a compatible type.
  ///
  /// @tparam U Source object type.
  /// @param other Pointer to move from.
  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr(intrusive_ptr<U>&& other) noexcept;

  /// @brief Releases the owned reference.
  ~intrusive_ptr();

  /// @brief Copies another pointer.
  ///
  /// @param other Pointer to copy.
  /// @return Reference to this pointer.
  intrusive_ptr& operator=(const intrusive_ptr& other);

  /// @brief Moves another pointer.
  ///
  /// @param other Pointer to move from.
  /// @return Reference to this pointer.
  intrusive_ptr& operator=(intrusive_ptr&& other) noexcept;

  /// @brief Copies a compatible pointer type.
  ///
  /// @tparam U Source object type.
  /// @param other Pointer to copy.
  /// @return Reference to this pointer.
  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr& operator=(const intrusive_ptr<U>& other);

  /// @brief Moves a compatible pointer type.
  ///
  /// @tparam U Source object type.
  /// @param other Pointer to move from.
  /// @return Reference to this pointer.
  template <class U>
    requires std::is_convertible_v<U*, T*>
  intrusive_ptr& operator=(intrusive_ptr<U>&& other) noexcept;

  /// @brief Releases the current reference and becomes empty.
  void reset();

  /// @brief Replaces the managed pointer.
  ///
  /// @param p Pointer to the new managed object.
  /// @param add_ref Whether to acquire a reference to the object.
  void reset(T* p, bool add_ref = true);

  /// @brief Returns the stored pointer.
  [[nodiscard]] T* get() const noexcept;

  /// @brief Releases ownership without decrementing the reference count.
  ///
  /// @return The previously stored pointer.
  [[nodiscard]] T* detach() noexcept;

  /// @brief Dereferences the managed object.
  [[nodiscard]] T& operator*() const noexcept;

  /// @brief Accesses the managed object.
  [[nodiscard]] T* operator->() const noexcept;

  /// @brief Checks whether the pointer contains an object.
  [[nodiscard]] explicit operator bool() const noexcept;

  /// @brief Swaps two intrusive pointers.
  ///
  /// @param other Pointer to swap with.
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

/// @brief Exchanges the managed objects of two intrusive pointers.
///
/// @tparam T The pointed-to type.
/// @param lhs First intrusive pointer.
/// @param rhs Second intrusive pointer.
template <class T>
constexpr void swap(intrusive_ptr<T>& lhs, intrusive_ptr<T>& rhs) noexcept {
  lhs.swap(rhs);
}

/// @brief Compares an intrusive pointer with `nullptr`.
template <class T>
constexpr bool operator==(const intrusive_ptr<T>& lhs,
                          const std::nullptr_t rhs) noexcept {
  return lhs.get() == rhs;
}

/// @brief Compares two intrusive pointers.
template <class T, class U>
constexpr bool operator==(const intrusive_ptr<T>& lhs,
                          const intrusive_ptr<U>& rhs) noexcept {
  return lhs.get() == rhs.get();
}

/// @brief Performs a three-way comparison of two intrusive pointers.
template <class T, class U>
constexpr std::strong_ordering operator<=>(
    const intrusive_ptr<T>& lhs, const intrusive_ptr<U>& rhs) noexcept {
  return std::compare_three_way{}(lhs.get(), rhs.get());
}

/// @brief Performs a three-way comparison of an intrusive pointer and null.
template <class T>
constexpr std::strong_ordering operator<=>(const intrusive_ptr<T>& lhs,
                                           std::nullptr_t) noexcept {
  return std::compare_three_way{}(lhs.get(), static_cast<T*>(nullptr));
}

/// @brief Performs a static cast between intrusive pointer types.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast.
/// @return Converted intrusive pointer.
template <class T, class U>
intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U> const& p) {
  return static_cast<T*>(p.get());
}

/// @brief Performs a static cast between intrusive pointer types.
///
/// Transfers ownership from the source pointer without modifying the
/// reference count.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast. The pointer is emptied after the cast.
/// @return Converted intrusive pointer owning the same object.
template <class T, class U>
intrusive_ptr<T> static_pointer_cast(intrusive_ptr<U>&& p) noexcept {
  return intrusive_ptr<T>(static_cast<T*>(p.detach()), false);
}

/// @brief Performs a const cast between intrusive pointer types.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast.
/// @return Converted intrusive pointer.
template <class T, class U>
intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U> const& p) {
  return const_cast<T*>(p.get());
}

/// @brief Performs a const cast between intrusive pointer types.
///
/// Transfers ownership from the source pointer without modifying the
/// reference count.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast. The pointer is empty after the cast.
/// @return Converted intrusive pointer owning the same object.
template <class T, class U>
intrusive_ptr<T> const_pointer_cast(intrusive_ptr<U>&& p) noexcept {
  return intrusive_ptr<T>(const_cast<T*>(p.detach()), false);
}

/// @brief Performs a dynamic cast between intrusive pointer types.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast.
/// @return Converted intrusive pointer.
template <class T, class U>
intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U> const& p) {
  return dynamic_cast<T*>(p.get());
}

/// @brief Performs a dynamic cast between intrusive pointer types.
///
/// Transfers ownership from the source pointer without modifying the
/// reference count if the cast succeeds.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast. The pointer is empty after a successful cast.
/// @return Converted intrusive pointer owning the same object, or an empty
///         pointer if the cast fails.
template <class T, class U>
intrusive_ptr<T> dynamic_pointer_cast(intrusive_ptr<U>&& p) noexcept {
  if (auto* ptr = dynamic_cast<T*>(p.get())) {
    p.detach();
    return intrusive_ptr<T>(ptr, false);
  }

  return {};
}

/// @brief Performs a reinterpret cast between intrusive pointer types.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast.
/// @return Converted intrusive pointer.
template <class T, class U>
intrusive_ptr<T> reinterpret_pointer_cast(const intrusive_ptr<U>& p) {
  return reinterpret_cast<T*>(p.get());
}

/// @brief Performs a reinterpret cast between intrusive pointer types.
///
/// Transfers ownership from the source pointer without modifying the
/// reference count.
///
/// @tparam T Target type.
/// @tparam U Source type.
/// @param p Pointer to cast. The pointer is empty after the cast.
/// @return Converted intrusive pointer owning the same object.
template <class T, class U>
intrusive_ptr<T> reinterpret_pointer_cast(intrusive_ptr<U>&& p) noexcept {
  return intrusive_ptr<T>(reinterpret_cast<T*>(p.detach()), false);
}

}  // namespace crimson_cell

/// @brief Hash support for intrusive_ptr.
template <typename T>
struct std::hash<crimson_cell::intrusive_ptr<T>> {
  size_t operator()(const crimson_cell::intrusive_ptr<T>& ptr) const noexcept {
    return std::hash<T*>{}(ptr.get());
  }
};