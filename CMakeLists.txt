cmake_minimum_required(VERSION 3.20)

project(SlimValue LANGUAGES CXX)

add_library(slim INTERFACE)

target_include_directories(slim
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(slim INTERFACE cxx_std_23)

option(SLIM_BUILD_TESTS "Build tests" ON)

if(SLIM_BUILD_TESTS)
    include(FetchContent)

    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
    )

    FetchContent_MakeAvailable(Catch2)

    enable_testing()

    add_executable(slim_tests
        tests/slim_tests.cpp
        tests/anyvalue_tests.cpp
        tests/errorinfo_tests.cpp
        tests/slimmap_tests.cpp
    )

    target_link_libraries(slim_tests
        PRIVATE
            slim
            Catch2::Catch2WithMain
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(slim_tests)

endif()