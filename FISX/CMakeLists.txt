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
    common/*.h
    collision/*.h
    FISX.h
)
file(GLOB SOURCE_FILES
    foundation/*.cpp
    foundation/*.h
    common/*.h
    collision/*.cpp
    collision/*.h
    FISX.cpp
    FISX.h
)

add_library(FISX_LIB STATIC ${SOURCE_FILES})

target_precompile_headers(FISX_LIB PRIVATE ${HEADER_FILES})

foreach(_source IN ITEMS ${SOURCE_FILES})
    if (IS_ABSOLUTE "${_source}")
        file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
    else()
        set(_source_rel "${_source}")
    endif()
    get_filename_component(_source_path "${_source_rel}" PATH)
    string(REPLACE "/" "\\" _group_path "${_source_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

target_include_directories(FISX_LIB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
