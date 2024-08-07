include_directories("${PROJECT_SOURCE_DIR}")

set(PROJECT_TEST_NAME ${LIB_NAME}-test)

if(DOWNLOAD_GTEST STREQUAL ON)
	# Download and unpack googletest at configure time
	configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
	  RESULT_VARIABLE result
	  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
	if(result)
	  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
	endif()
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
	  RESULT_VARIABLE result
	  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
	if(result)
	  message(FATAL_ERROR "Build step for googletest failed: ${result}")
	endif()
	# Prevent overriding the parent project's compiler/linker
	# settings on Windows
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	# Add googletest directly to our build. This defines
	# the gtest and gtest_main targets.
	add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
		         ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
		         EXCLUDE_FROM_ALL)

endif()

add_executable(${PROJECT_TEST_NAME} 
		invalid_xml.unit.cpp
		InputDifferentEmissivities.unit.cpp
		InputInvertedEmissivities.unit.cpp
		InputFile1.unit.cpp
		read_json_file_from_disk.unit.cpp
		read_optics_file_from_disk.unit.cpp
		convert_optics_to_json.unit.cpp
		read_igsdb_shading_layer_json.unit.cpp
		read_bsdf_xml_file.unit.cpp
		main.cpp 
		paths.h )

include_directories(${PROJECT_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_TEST_NAME} gmock_main ${LIB_NAME})

target_compile_features(${PROJECT_TEST_NAME} PRIVATE cxx_std_17)

add_test(NAME ${PROJECT_TEST_NAME}-runner COMMAND ${PROJECT_TEST_NAME} "${CMAKE_CURRENT_LIST_DIR}")

