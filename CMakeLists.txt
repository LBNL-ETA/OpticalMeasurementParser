cmake_minimum_required( VERSION 2.8.7 )

set( target_name OpticalMeasurementParser )

set (CMAKE_CXX_STANDARD 11)

project( ${target_name} )

include( cmake/WCEProjectMacros.cmake )
include( cmake/WCEInternalUtils.cmake )
include( cmake/WCECompilerFlags.cmake )

# google test will not be created by this project if this is not main project. Main project is expected to handle google test.
get_directory_property(hasParent PARENT_DIRECTORY)

if(hasParent)
  set( BUILD_PAR_TESTING OFF )
  set( DOWNLOAD_GTEST OFF )
  if( BUILD_TESTING STREQUAL ON ) # EnergyPlus testing is ON
    set( BUILD_PAR_TESTING ON )
  endif()
	set( SINGLE_PROJECT ON )
else()
	option( BUILD_PAR_TESTING "Build testing targets" ON )
	option( SINGLE_PROJECT "Build windows library as single project" OFF )
	set( DOWNLOAD_GTEST ON )
endif()

if( DOWNLOAD_GTEST STREQUAL ON ) # Do not need gtest since it will be processed by E+
	# include(CMakeLists-GTest.txt)
	configure_file(CMakeLists.txt.in ${CMAKE_BINARY_DIR}/googletest-download/CMakeLists.txt)
	execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
	execute_process(COMMAND ${CMAKE_COMMAND} --build .
  		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download )
  		
  # Prevent GoogleTest from overriding our compiler/linker options
	# when building with Visual Studio
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 
	# Add googletest directly to our build. This adds
	# the following targets: gtest, gtest_main, gmock
	# and gmock_main
	add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
    	${CMAKE_BINARY_DIR}/googletest-build)
 
	# Now simply link your own targets against gtest, gmock,
	# etc. as appropriate
  		
    include_directories( ${CMAKE_BINARY_DIR}/googletest-src/include )
endif ()

file( GLOB SOURCES_CPP "src/*.cpp" )
file( GLOB SOURCES_HPP "src/*.hpp" )

LIST(APPEND SOURCES ${SOURCES_HPP} ${SOURCES_CPP} )

add_library( ${target_name} STATIC ${SOURCES} )

include_directories( include )

if( BUILD_PAR_TESTING )
    file( GLOB all_test_src RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" tst/*.cpp )
    set( test_src ${all_test_src} )
    CREATE_TEST_TARGETS_PAR( ${target_name} "${test_src}" "" )
endif ()

config_compiler_and_linker_par()