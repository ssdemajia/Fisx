configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

file(GLOB SOURCE_FILES
    *.cpp
)

add_executable(Demo main.cpp)

target_link_libraries(
    Demo
    FISX_LIB
)