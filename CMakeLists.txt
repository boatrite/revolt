cmake_minimum_required(VERSION 2.8.12)
project(VoxelEngineMk2)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(ve2 src/main.cpp)
target_link_libraries(ve2 ${CONAN_LIBS})

