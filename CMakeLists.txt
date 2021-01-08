cmake_minimum_required(VERSION 3.0)
project(VoxelEngineMk2 CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable( ve2
                src/bindings/imgui_impl_glfw.cpp
                src/bindings/imgui_impl_opengl3.cpp
                src/bindings/imgui_impl_glfw.h
                src/bindings/imgui_impl_opengl3.h
                src/main.cpp
                src/window.cpp
)
target_link_libraries(ve2 ${CONAN_LIBS})
