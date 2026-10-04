[![CI](https://github.com/Patrix9999/intrusive_ptr/actions/workflows/verify.yml/badge.svg)](https://github.com/Patrix9999/intrusive_ptr/actions/workflows/verify.yml)
[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.cppreference.com/w/cpp/20)
[![License](https://img.shields.io/github/license/Patrix9999/intrusive_ptr)](https://github.com/Patrix9999/intrusive_ptr/blob/master/LICENSE)
[![Release](https://img.shields.io/github/v/release/Patrix9999/intrusive_ptr)](https://github.com/Patrix9999/intrusive_ptr/releases)
[![codecov](https://codecov.io/gh/Patrix9999/intrusive_ptr/graph/badge.svg)](https://codecov.io/gh/Patrix9999/intrusive_ptr)

## About

**intrusive_ptr** is a lightweight, header-only intrusive smart pointer implementation for modern C++.

It provides reference-counted ownership without requiring a separate control block, keeping the smart pointer itself to a single pointer-sized member. Reference counting is stored directly in the managed object, making the library suitable for applications where minimal overhead and predictable memory usage are important.

The library is designed to be simple, modern, and easy to integrate into existing C++ projects.

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


## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.