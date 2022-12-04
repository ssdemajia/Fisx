cmake_minimum_required(VERSION 3.16)

project(FISX LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)

set(FISX_VERSION_MAJOR 0)
set(FISX_VERSION_MINOR 1)
set(FISX_VERSION_PATCH 0)

add_subdirectory(FISX)
add_subdirectory(Demo)

# USE_FOLDERS group cmake generated projects into one (CMakePredefinedTargets) folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Below line set Demo as startup application in Visual Studio
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo)

