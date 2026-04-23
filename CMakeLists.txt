cmake_minimum_required(VERSION 3.20)

cmake_path(GET CMAKE_SOURCE_DIR FILENAME SLIM_LIB)
string(TOLOWER "${SLIM_LIB}" SLIM_LIB_LOWER)

if(DEFINED CPACK_OUTPUT_FILE_PREFIX)
    if("${CPACK_OUTPUT_FILE_PREFIX}" STREQUAL ".")
	    set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_SOURCE_DIR})
    endif()
else()
	message(FATAL_ERROR "CPACK_OUTPUT_FILE_PREFIX is not set")
endif()

include(FetchContent)

# ------------------------------------------------------------
# Local override controls (added only)
# ------------------------------------------------------------
set(SLIM_USE_LOCAL_SOURCE OFF CACHE BOOL "Use local SlimValue source instead of git")

if(SLIM_USE_LOCAL_SOURCE)
    set(GIT_TAG 0.0.0)
    set(SLIMVALUE_GIT_HASH "none")
else()
    execute_process(
        COMMAND git ls-remote --tags --sort=-v:refname https://github.com/greergan/${SLIM_LIB}.git
        OUTPUT_VARIABLE TAGS_RAW
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )

    string(REGEX MATCH "refs/tags/([^\\^\\n]+)" _ "${TAGS_RAW}")
    set(GIT_TAG ${CMAKE_MATCH_1})
endif()

# ------------------------------------------------------------
# FetchContent guarded (only change: conditional)
# ------------------------------------------------------------
if(SLIM_USE_LOCAL_SOURCE)
    set(slimvaluesrc_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
    message(STATUS "using LOCAL source (${slimvaluesrc_SOURCE_DIR})")
else()
    FetchContent_Declare(
        SlimValueSrc
        GIT_REPOSITORY https://github.com/greergan/SlimValue.git
        GIT_TAG        ${GIT_TAG}
    )
    message(STATUS "using GIT source (tag: ${GIT_TAG})")
    message(STATUS "fetched from https://github.com/greergan/SlimValue.git")
    FetchContent_Populate(SlimValueSrc)
        execute_process(
        COMMAND git rev-parse --short HEAD
        OUTPUT_VARIABLE SLIMVALUE_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ERROR_QUIET
    )
endif()

project(SlimValue VERSION ${GIT_TAG} LANGUAGES CXX)

message(STATUS "OS: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "Project dir: ${CMAKE_SOURCE_DIR}")
message(STATUS "Build dir:   ${CMAKE_BINARY_DIR}")
message(STATUS "Dist dir: ${CPACK_OUTPUT_FILE_PREFIX}")
message(STATUS "slim library name: ${SLIM_LIB}")
message(STATUS ".pc.in stub name: ${SLIM_LIB_LOWER}")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)


set(SLIMVALUE_VERSION ${GIT_TAG})
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
    "${slimvaluesrc_SOURCE_DIR}/slimvalue.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/slimvalue.pc"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/slimvalue.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" ARCH)
if(ARCH MATCHES "x86_64|amd64")
    set(ARCH_NAME "x86_64")
elseif(ARCH MATCHES "i386|i686")
    set(ARCH_NAME "x86")
elseif(ARCH MATCHES "aarch64|arm64")
    set(ARCH_NAME "arm64")
elseif(ARCH MATCHES "armv7|arm")
    set(ARCH_NAME "arm")
else()
    set(ARCH_NAME "unknown")
endif()

set(CPACK_GENERATOR "DEB;RPM")

set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}-${ARCH_NAME}")

set(CPACK_PACKAGE_NAME "slimvalue")
set(CPACK_PACKAGE_VERSION ${GIT_TAG})
set(CPACK_PACKAGE_CONTACT "greergan@gmail.com")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Jeff Greer")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH_NAME})

set(CPACK_RPM_PACKAGE_NAME "slimvalue")
set(CPACK_RPM_PACKAGE_VERSION ${GIT_TAG})
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_SUMMARY "SlimValue header-only C++ library")
set(CPACK_RPM_PACKAGE_ARCHITECTURE ${ARCH_NAME})

include(CPack)