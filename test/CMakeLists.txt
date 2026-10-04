include(FetchContent)

FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.18.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

add_executable(test)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    "*.hpp"
    "*.cpp"
)

target_sources(test
    PRIVATE
        ${SRC}
)

target_include_directories(test
    PRIVATE
        "./"
)

target_link_libraries(test
    PRIVATE
        intrusive_ptr
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test)