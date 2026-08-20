cmake_minimum_required(VERSION 3.8)

project(OpticalMeasurementParser VERSION 2.3.11 LANGUAGES CXX)
set(LIB_NAME ${PROJECT_NAME})

# Local development mode. The `local` preset sets LBNL_LOCAL_SIBLINGS=ON (a cache
# variable, so it cascades into FetchContent sub-builds). When it is on, prefer a
# sibling working copy (../<Name>) of each of THIS project's direct dependencies,
# so the whole dependency graph can be built from local disk during development.
# A dependency whose sibling is not checked out falls back to its declared remote,
# independently. Propagation: a sibling dependency that also honors this flag wires
# up its own siblings in turn. When the flag is off (e.g. the `default` preset),
# nothing here runs and every dependency comes from its remote.
if(LBNL_LOCAL_SIBLINGS)
    foreach(_sibling "json=NLOHMANN_JSON" "BSDFXMLParser=BSDFXMLPARSER" "googletest=GOOGLETEST")
        string(REPLACE "=" ";" _sibling "${_sibling}")
        list(GET _sibling 0 _sibling_dir)
        list(GET _sibling 1 _sibling_var)
        if(NOT FETCHCONTENT_SOURCE_DIR_${_sibling_var}
           AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../${_sibling_dir}/CMakeLists.txt")
            set(FETCHCONTENT_SOURCE_DIR_${_sibling_var}
                "${CMAKE_CURRENT_SOURCE_DIR}/../${_sibling_dir}")
            message(STATUS "[local] ${_sibling_dir} -> ${FETCHCONTENT_SOURCE_DIR_${_sibling_var}}")
        endif()
    endforeach()
endif()

if(NOT "${CMAKE_CXX_STANDARD}")
    set(CMAKE_CXX_STANDARD 20)
endif()

if(NOT "${CMAKE_CXX_EXTENSIONS}")
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

# Enable MSVC parallel (multi-processor) compilation for all targets and
# dependencies defined below. Placed before add_subdirectory/FetchContent so
# every later target inherits it (add_compile_options only affects later targets).
if(MSVC)
    add_compile_options(/MP)
endif()

include(GNUInstallDirs)
if(NOT "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()

if(NOT "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
endif()

if(NOT "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
endif()

# Check to see if this project is included in another via add_subdirectory
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(DOWNLOAD_GTEST ON)
else()
    set(DOWNLOAD_GTEST OFF)
endif()

set(JSON_BuildTests OFF CACHE BOOL "")

include(FetchContent)

set(nlohmann_Branch "v3.12.0")

if(NOT TARGET nlohmann_json)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG ${nlohmann_Branch}
    )
    FetchContent_MakeAvailable(nlohmann_json)
endif()


set(BUILD_BSDFXMLParser_Tests OFF CACHE BOOL "Build BSDFXMLParser tests" FORCE)

set(BSDFXMLParser_Branch "Version_0.0.13")

if(NOT TARGET BSDFXMLParser)
    FetchContent_Declare(
        BSDFXMLParser
        GIT_REPOSITORY https://github.com/LBNL-ETA/BSDFXMLParser.git
        GIT_TAG ${BSDFXMLParser_Branch}
    )
    FetchContent_MakeAvailable(BSDFXMLParser)
endif()
FetchContent_GetProperties(BSDFXMLParser)

# Set include directories for FileParse
set(BSDFXMLParser_INCLUDE_DIRS "${bsdfxmlparser_SOURCE_DIR}/src")
include_directories(${BSDFXMLParser_INCLUDE_DIRS})

# Add your target
add_subdirectory(src)

# Add nlohmann_json include directory to your target
target_include_directories(${LIB_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${nlohmann_json_SOURCE_DIR}/include
        $<BUILD_INTERFACE:${bsdfxmlparser_SOURCE_DIR}/src>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

Option(BUILD_Optical_Measurement_Parser_Tests "Build tests for optical measurement parsing." ON)

if(BUILD_Optical_Measurement_Parser_Tests)
    enable_testing()
    add_subdirectory(test)

    # Set the path to the test directory
    set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
    target_compile_definitions(OpticalMeasurementParser-test PRIVATE TEST_DATA_DIR="${TEST_DATA_DIR}")

    # Add the test with the specified arguments
    add_test(NAME OpticalMeasurementParser-test
             COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/OpticalMeasurementParser-test --gtest_catch_exceptions=0)
endif()

get_property(fileParseFromBSDFXMLParser TARGET BSDFXMLParser PROPERTY FileParse_Branch)
configure_file(doc/OpticalMeasurementParser.md.in ${CMAKE_CURRENT_SOURCE_DIR}/doc/OpticalMeasurementParser.md @ONLY)

# Setting variable for parent projects so it can be checked if same version is used
set_property(TARGET ${LIB_NAME} PROPERTY BSDFXMLParser_Branch ${BSDFXMLParser_Branch})
set_property(TARGET ${LIB_NAME} PROPERTY nlohmann_Branch ${nlohmann_Branch})