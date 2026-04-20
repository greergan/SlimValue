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
    find_package(Catch2 3 REQUIRED)

    enable_testing()

    add_executable(slim_tests
        tests/slim_tests.cpp
    )

    target_link_libraries(slim_tests
        PRIVATE
            slim
    )

    target_link_libraries(slim_tests
        PRIVATE slim Catch2::Catch2WithMain
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(slim_tests)
endif()
