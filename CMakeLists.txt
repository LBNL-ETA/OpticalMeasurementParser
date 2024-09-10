cmake_minimum_required(VERSION 3.8)

project(OpticalMeasurementParser VERSION 2.1.0 LANGUAGES CXX)
set(LIB_NAME ${PROJECT_NAME})

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(NOT "${CMAKE_CXX_STANDARD}")
    set(CMAKE_CXX_STANDARD 14)
endif()

if(NOT "${CMAKE_CXX_EXTENSIONS}")
    set(CMAKE_CXX_EXTENSIONS OFF)
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
set(BUILD_FileParse_tests OFF CACHE BOOL "")

include(FetchContent)

if(NOT TARGET nlohmann_json)
    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG "db53bda"
    )

    FetchContent_MakeAvailable(nlohmann_json)
endif()

if(NOT TARGET xmlParser)
    FetchContent_Declare(
        xmlParser
        GIT_REPOSITORY https://github.com/LBNL-ETA/XMLParser.git
        GIT_TAG "v1.0.1"
    )

    FetchContent_MakeAvailable(xmlParser)
endif()

if(NOT TARGET FileParse)
    FetchContent_Declare(
        FileParse
        GIT_REPOSITORY https://github.com/LBNL-ETA/FileParse.git
        GIT_TAG BaseLoaderRoutines
    )

    FetchContent_MakeAvailable(FileParse)
endif()

# Set include directories for FileParse
set(FileParse_INCLUDE_DIRS "${fileparse_SOURCE_DIR}/include")
include_directories(${FileParse_INCLUDE_DIRS})

# Add your target
add_subdirectory(src)

# Add nlohmann_json include directory to your target
target_include_directories(${LIB_NAME}
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        ${nlohmann_json_SOURCE_DIR}/include
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
