[![CI](https://github.com/Patrix9999/intrusive_ptr/actions/workflows/verify.yml/badge.svg)](https://github.com/Patrix9999/intrusive_ptr/actions/workflows/verify.yml)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License](https://img.shields.io/github/license/Patrix9999/intrusive_ptr)](https://github.com/Patrix9999/intrusive_ptr/blob/master/LICENSE)
[![Release](https://img.shields.io/github/v/release/Patrix9999/intrusive_ptr)](https://github.com/Patrix9999/intrusive_ptr/releases)
[![codecov](https://codecov.io/gh/Patrix9999/intrusive_ptr/graph/badge.svg)](https://codecov.io/gh/Patrix9999/intrusive_ptr)

## About

[crimson_cell::intrusive_ptr](https://github.com/Patrix9999/intrusive_ptr) is a lightweight, header-only intrusive smart pointer implementation for modern C++.

It provides reference-counted ownership without requiring a separate control block, keeping the smart pointer itself to a single pointer-sized member. Reference counting is stored directly in the managed object, making the library suitable for applications where minimal overhead and predictable memory usage are important.

The library is designed to be simple, modern, and easy to integrate into existing projects.

## Why use it?

`intrusive_ptr` is intended for class objects that implement their own reference counting. The reference count is stored directly in the object, allowing the smart pointer to participate in the object's existing lifetime-management mechanism.

This is particularly useful when working with APIs or object models that already expose `AddRef`/`Release`-style ownership.

Using [std::shared_ptr](https://en.cppreference.com/cpp/memory/shared_ptr) in such systems can require maintaining two separate ownership mechanisms: the object's internal reference count and `shared_ptr`'s external control block. Keeping both mechanisms synchronized is tedious and fragile, and can lead to incorrect lifetime management.

With `intrusive_ptr`:

* **The object owns its reference count** — `AddRef`/`Release` operate directly on the managed object.
* **No duplicated ownership state** — There is no separate `shared_ptr` control block that must be kept in sync.
* **Minimal footprint** — The `intrusive_ptr` itself contains only a single pointer.
* **No additional allocation** — No separate control block is required.
* **Works with existing reference-counted objects** — Particularly useful for COM-style and other intrusive reference-counted APIs.
* **Header-only** — No library to build or link against.

The trade-off is that the managed type must explicitly participate in the reference-counting mechanism.

## Why not use it?

`intrusive_ptr` is designed for objects that implement their own reference-counting mechanism. If your object does not already use intrusive reference counting, [std::shared_ptr](https://en.cppreference.com/cpp/memory/shared_ptr) is usually the more convenient choice.

You may want to use `std::shared_ptr` instead when:

* Your object does not implement its own reference counting.
* You need **thread-safe** reference-count manipulation out of the box.
* You want to keep lifetime management completely separate from the managed object's implementation.
* You need to manage arbitrary types without requiring them to participate in the ownership mechanism.

In short, `intrusive_ptr` is best suited for objects that already own their reference-counting mechanism. If you simply need shared ownership of an otherwise ordinary type, `std::shared_ptr` is generally the simpler choice.

## Installation

Download the header from the [Releases](https://github.com/Patrix9999/intrusive_ptr/releases) page and include `crimson_cell/intrusive_ptr.hpp`.

Alternatively you can also add the repository as a `git submodule` and include it in your CMake project via [add_subdirectory](https://cmake.org/cmake/help/latest/command/add_subdirectory.html) function.

## Usage Example

Define `intrusive_ptr_add_ref` and `intrusive_ptr_release` to connect it to the object's `AddRef()` and `Release()` methods.

```cpp
#include <crimson_cell/intrusive_ptr.hpp>
#include "RefCountedObject.hpp"

void intrusive_ptr_add_ref(RefCountedObject* object) noexcept {
    object->AddRef();
}

void intrusive_ptr_release(RefCountedObject* object) noexcept {
    object->Release();
}

int main() {
    auto ptr = crimson_cell::intrusive_ptr<RefCountedObject>{new RefCountedObject};

    return 0;
}
```

## Unit Tests

The project includes a comprehensive unit test suite built with [GoogleTest](https://github.com/google/googletest).

Those tests cover the core functionality of `intrusive_ptr`, including:

* Reference counting and ownership semantics
* Copy and move operations
* Pointer conversions
* Reset and detach operations
* Pointer comparisons and casts
* Reference-counted object lifetime
* Edge cases and regression tests

## Automation

The project uses [GitHub Actions](https://github.com/features/actions) to automate development and release workflows.

The [verify.yml](.github/workflows/verify.yml) workflow runs automatically on every push and pull request. It checks code formatting, builds the library on **Linux** and **Windows**, and runs the unit test suite on those platforms.

Release workflows automate the process of publishing new versions, helping keep releases consistent and reproducible.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.