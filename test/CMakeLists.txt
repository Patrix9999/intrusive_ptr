include(GoogleTest)
include(FetchContent)

block(PROPAGATE GTest::gtest_main)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG v1.18.0
    )

    # For Windows: Prevent overriding the parent project's compiler/linker settings
    if (WIN32)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()

    FetchContent_MakeAvailable(googletest)
endblock()

add_executable(intrusive_ptr_test)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS
    "*.hpp"
    "*.cpp"
)

target_sources(intrusive_ptr_test
    PRIVATE
        ${SRC}
)

target_include_directories(intrusive_ptr_test
    PRIVATE
        "./"
)

target_link_libraries(intrusive_ptr_test
    PRIVATE
        intrusive_ptr
        GTest::gtest_main
)

gtest_discover_tests(intrusive_ptr_test)