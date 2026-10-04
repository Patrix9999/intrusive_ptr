cmake_minimum_required(VERSION 3.21)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(intrusive_ptr INTERFACE)

target_sources(intrusive_ptr
	INTERFACE
		"include/intrusive_ptr.hpp"
)

target_include_directories(intrusive_ptr
	INTERFACE
		"include/"
)