set( CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON )

add_library( ${LIB_NAME} 
		Parser.hpp
		Parser.cpp
		ProductData.hpp
		ProductData.cpp)



if(NOT "${fPIC}")
  set(fPIC ON)
endif()

if(fPIC)
  set_property(TARGET ${LIB_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)
endif()

target_compile_options(${LIB_NAME} PRIVATE -Werror)

