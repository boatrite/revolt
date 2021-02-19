cmake_minimum_required(VERSION 3.11)

project(Revolt CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB_RECURSE SOURCES src/*.cpp extern/*.cpp)
add_executable(revolt ${SOURCES})

add_compile_options("-pedantic-errors -Wall -Weffc++ -Wextra -Wsign-conversion")

file(GLOB_RECURSE SHADERS src/*.vert src/*.frag)
file(GLOB_RECURSE FONTS src/assets/*.ttf)
add_custom_command(TARGET revolt
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${SHADERS} ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${FONTS} ${PROJECT_BINARY_DIR}
)

target_link_libraries(revolt ${CONAN_LIBS})
