cmake_minimum_required(VERSION 3.10)

# Remove trailing '\' and '/' at the end of the path, so can reliably compare
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# When compiling DLib as stand alone library set to C++11,
# when library is part of other project, respect parent project settings.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	# Enforce c++11 standard.
	set (CMAKE_CXX_STANDARD 17)
endif()

file(GLOB HEADER_FILES
    foundation/*.h
    FISX.h
)
file(GLOB SOURCE_FILES
    foundation/*.cpp
    FISX.cpp
)
add_library(FISX_LIB STATIC ${SOURCE_FILES})
target_include_directories(FISX_LIB PUBLIC ${HEADER_FILES})
