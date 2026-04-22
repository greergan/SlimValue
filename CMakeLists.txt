cmake_minimum_required(VERSION 3.20)

include(FetchContent)

execute_process(
    COMMAND git ls-remote --tags --sort=-v:refname https://github.com/greergan/SlimValue.git
    OUTPUT_VARIABLE TAGS_RAW
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

string(REGEX MATCH "refs/tags/([^\\^\\n]+)" _ "${TAGS_RAW}")
set(GIT_TAG ${CMAKE_MATCH_1})

if(NOT GIT_TAG)
    set(GIT_TAG main)
endif()

FetchContent_Declare(
    SlimValueSrc
    GIT_REPOSITORY https://github.com/greergan/SlimValue.git
    GIT_TAG        ${GIT_TAG}
)

project(SlimValue VERSION ${GIT_TAG} LANGUAGES CXX)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

FetchContent_Populate(SlimValueSrc)

set(SLIMVALUE_VERSION ${GIT_TAG})

execute_process(
    COMMAND git rev-parse --short HEAD
    OUTPUT_VARIABLE SLIMVALUE_GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

set(SLIMVALUE_VERSION_FULL "${SLIMVALUE_VERSION}+${SLIMVALUE_GIT_HASH}")

configure_file(
    "${slimvaluesrc_SOURCE_DIR}/include/slim/SlimValue.hpp.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/slim/SlimValue.hpp"
    @ONLY
)

add_library(slim INTERFACE)

target_include_directories(slim
    INTERFACE
        $<BUILD_INTERFACE:${slimvaluesrc_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(slim INTERFACE cxx_std_23)
set(SLIM_CXX_STANDARD 23)

option(SLIM_BUILD_TESTS "Build tests" ON)

if(SLIM_BUILD_TESTS)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.5.0
    )

    FetchContent_MakeAvailable(Catch2)

    enable_testing()

    add_executable(slim_tests
        ${slimvaluesrc_SOURCE_DIR}/tests/slim_tests.cpp
        ${slimvaluesrc_SOURCE_DIR}/tests/anyvalue_tests.cpp
        ${slimvaluesrc_SOURCE_DIR}/tests/errorinfo_tests.cpp
        ${slimvaluesrc_SOURCE_DIR}/tests/slimmap_tests.cpp
        ${slimvaluesrc_SOURCE_DIR}/tests/slimvalue_tests.cpp
    )

    target_link_libraries(slim_tests
        PRIVATE slim Catch2::Catch2WithMain
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(slim_tests)
endif()

install(TARGETS slim EXPORT SlimTargets)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/include/slim/SlimValue.hpp"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/slim
)

install(EXPORT SlimTargets
        FILE SlimTargets.cmake
        NAMESPACE Slim::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SlimValue)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SlimValueConfigVersion.cmake"
    VERSION ${GIT_TAG}
    COMPATIBILITY AnyNewerVersion
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/SlimValueConfig.cmake"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/SlimTargets.cmake\")\n")

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/SlimValueConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/SlimValueConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SlimValue
)

configure_file(
    "${slimvaluesrc_SOURCE_DIR}/cmake/slimvalue.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/slimvalue.pc"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/slimvalue.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

set(CPACK_GENERATOR "DEB;RPM")

set(CPACK_PACKAGE_NAME "slimvalue")
set(CPACK_PACKAGE_VERSION ${GIT_TAG})
set(CPACK_PACKAGE_CONTACT "greergan@gmail.com")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jeff Greer")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "all")

set(CPACK_RPM_PACKAGE_NAME "slimvalue")
set(CPACK_RPM_PACKAGE_VERSION ${GIT_TAG})
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_SUMMARY "SlimValue header-only C++ library")
set(CPACK_RPM_PACKAGE_ARCHITECTURE "noarch")

include(CPack)