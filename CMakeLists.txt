cmake_minimum_required(VERSION 3.16)

project(slim LANGUAGES CXX)

add_library(slim INTERFACE)
target_include_directories(slim INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_compile_features(slim INTERFACE cxx_std_23)
