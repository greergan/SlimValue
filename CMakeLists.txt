cmake_minimum_required(VERSION 3.20)

execute_process(
    COMMAND git tag
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_TAG
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)

if(NOT GIT_TAG)
    set(GIT_TAG 0.0.0)
endif()

project(SlimValue VERSION ${GIT_TAG} LANGUAGES CXX)

include(GNUInstallDirs)

add_library(slim INTERFACE)

target_include_directories(slim
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(slim INTERFACE cxx_std_23)

# install targets
install(TARGETS slim EXPORT SlimTargets)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT SlimTargets
        FILE SlimTargets.cmake
        NAMESPACE Slim::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/SlimValue)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/SlimValueConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
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
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/slimvalue.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/slimvalue.pc"
    @ONLY
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/slimvalue.pc"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

set(CPACK_GENERATOR "DEB;RPM")

set(CPACK_PACKAGE_NAME "slimvalue")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "you@example.com")

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "you")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")

set(CPACK_RPM_PACKAGE_NAME "slimvalue")
set(CPACK_RPM_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_RPM_PACKAGE_RELEASE "1")
set(CPACK_RPM_PACKAGE_LICENSE "MIT")
set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
set(CPACK_RPM_PACKAGE_SUMMARY "SlimValue header-only C++ library")
set(CPACK_RPM_PACKAGE_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_RPM_PACKAGE_REQUIRES "")

if(DEFINED CPACK_OUTPUT_FILE_PREFIX)
    if(CPACK_OUTPUT_FILE_PREFIX STREQUAL ".")
        set(CPACK_OUTPUT_FILE_PREFIX "${PROJECT_SOURCE_DIR}")
    else ()
        set(CPACK_OUTPUT_FILE_PREFIX "${CPACK_OUTPUT_FILE_PREFIX}")
    endif()
endif()

include(CPack)

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