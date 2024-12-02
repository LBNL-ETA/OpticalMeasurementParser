set( CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON )

add_library( ${LIB_NAME}
        factories/bsdfxml.cpp
        factories/bsdfxml.hpp
        Parser.hpp
		Parser.cpp
		ProductData.hpp
		ProductData.cpp)
		
# Create a wrapper interface library for nlohmann_json
add_library(nlohmann_json_custom INTERFACE)

# Link the original nlohmann_json target to the custom one
target_link_libraries(nlohmann_json_custom INTERFACE nlohmann_json::nlohmann_json)

# Add compile options to suppress the warnings for GCC and Clang
if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(nlohmann_json_custom INTERFACE -Wno-type-limits)
endif()

target_link_libraries(${LIB_NAME} PRIVATE nlohmann_json_custom)
target_link_libraries(${LIB_NAME} PRIVATE xmlParser)
target_link_libraries(${LIB_NAME} PRIVATE FileParse)
target_link_libraries(${LIB_NAME} PRIVATE BSDFXMLParser)

if(NOT "${fPIC}")
  set(fPIC ON)
endif()

if(fPIC)
  set_property(TARGET ${LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()


if (CMAKE_COMPILER_IS_GNUCC)
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror) 
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${LIB_NAME} PRIVATE -Wall -Wextra -pedantic -Werror )
endif()
if (MSVC)
    target_compile_options(${LIB_NAME} PRIVATE /W4 /WX)
endif()

target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
