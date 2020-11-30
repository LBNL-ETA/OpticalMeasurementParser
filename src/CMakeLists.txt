set( CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON )

add_library( ${LIB_NAME} 
		Parser.hpp
		Parser.cpp
		ProductData.hpp
		ProductData.cpp
		xmlParser.h
		xmlParser.cpp)

target_link_libraries(${LIB_NAME} PUBLIC nlohmann_json::nlohmann_json)

if(NOT "${fPIC}")
  set(fPIC ON)
endif()

if(fPIC)
  set_property(TARGET ${LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()


if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror) 
	set_source_files_properties(xmlParser.cpp PROPERTIES COMPILE_FLAGS -Wno-error)# Disable treating warnings as errors for XMLParser.  It is 3rd party library code
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror )
	set_source_files_properties(xmlParser.cpp PROPERTIES COMPILE_FLAGS -Wno-error) # Disable treating warnings as errors for XMLParser.  It is 3rd party library code
endif()
if (MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /W4 /WX)
	set_source_files_properties(xmlParser.cpp PROPERTIES COMPILE_FLAGS /WX-) # Disable treating warnings as errors for XMLParser.  It is 3rd party library code

endif()


target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
