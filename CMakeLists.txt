cmake_minimum_required(VERSION 3.21)

project(intrusive_ptr)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(INTRUSIVE_PTR_ENABLE_TEST "This option enables unit test project. By default this option is set to ON" ON)

add_library(intrusive_ptr INTERFACE)
add_library(crimson_cell::intrusive_ptr ALIAS intrusive_ptr)

target_sources(intrusive_ptr
	INTERFACE
		"include/crimson_cell/intrusive_ptr.hpp"
)

target_include_directories(intrusive_ptr
	INTERFACE
		"include/"
)

if (INTRUSIVE_PTR_ENABLE_TEST)
	enable_testing()
	add_subdirectory(test)
endif()