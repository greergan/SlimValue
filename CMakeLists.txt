cmake_minimum_required(VERSION 3.20)

project(SlimValue LANGUAGES CXX)

include(FetchContent)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# ------------------------------------------------------------
# Resolve latest tag (remote)
# ------------------------------------------------------------
execute_process(
    COMMAND git ls-remote --tags --sort=-v:refname https://github.com/greergan/SlimValue.git
    OUTPUT_VARIABLE TAGS_RAW
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

string(REGEX MATCH "refs/tags/([^\\^\\n]+)" _ "${TAGS_RAW}")
set(LATEST_TAG ${CMAKE_MATCH_1})

if(NOT LATEST_TAG)
    set(LATEST_TAG main)
endif()

# ------------------------------------------------------------
# Fetch source ONLY (no upstream CMake execution)
# ------------------------------------------------------------
FetchContent_Declare(
    SlimValueSrc
    GIT_REPOSITORY https://github.com/greergan/SlimValue.git
    GIT_TAG        ${LATEST_TAG}
)

FetchContent_Populate(SlimValueSrc)

# ------------------------------------------------------------
# Library (your wrapper target)
# ------------------------------------------------------------
add_library(slim INTERFACE)

target_include_directories(slim
    INTERFACE
        $<BUILD_INTERFACE:${slimvaluesrc_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(slim INTERFACE cxx_std_23)

# ------------------------------------------------------------
# Tests (your control, not upstream)
# ------------------------------------------------------------
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
    )

    target_link_libraries(slim_tests
        PRIVATE slim Catch2::Catch2WithMain
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(slim_tests)
endif()

# ------------------------------------------------------------
# Install / Packaging
# ------------------------------------------------------------
install(TARGETS slim EXPORT SlimTargets)

install(DIRECTORY ${slimvaluesrc_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT SlimTargets
        FILE SlimTargets.cmake
        NAMESPACE Slim::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SlimValue)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SlimValueConfigVersion.cmake"
    VERSION ${LATEST_TAG}
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

# ------------------------------------------------------------
# CPack
# ------------------------------------------------------------
set(CPACK_GENERATOR "DEB;RPM")

set(CPACK_PACKAGE_NAME "slimvalue")
set(CPACK_PACKAGE_VERSION ${LATEST_TAG})
set(CPACK_PACKAGE_CONTACT "you@example.com")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "you")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

set(CPACK_RPM_PACKAGE_NAME "slimvalue")
set(CPACK_RPM_PACKAGE_VERSION ${LATEST_TAG})
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_SUMMARY "SlimValue header-only C++ library")
set(CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

include(CPack)