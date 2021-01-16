cmake_minimum_required(VERSION 3.0)
project(VoxelEngineMk2 CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions("-std=c++17")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable( ve2
                src/app.cpp
                src/app.h
                src/bindings/imgui_impl_glfw.cpp
                src/bindings/imgui_impl_glfw.h
                src/bindings/imgui_impl_opengl3.cpp
                src/bindings/imgui_impl_opengl3.h
                src/camera.cpp
                src/camera.h
                src/font_book.h
                src/main.cpp
                src/renderers/coordinate_lines_renderer.h
                src/renderers/demo_renderer.cpp
                src/renderers/demo_renderer.h
                src/renderers/main_menu_renderer.cpp
                src/renderers/main_menu_renderer.h
                src/renderers/overlay_renderer.cpp
                src/renderers/overlay_renderer.h
                src/renderers/renderer.h
                src/renderers/root_renderer.cpp
                src/renderers/root_renderer.h
                src/ui_context.cpp
                src/ui_context.h
                src/util/file.cpp
                src/util/file.h
                src/util/shader.cpp
                src/util/shader.h
                src/window.h
)

add_compile_options("-pedantic-errors -Wall -Weffc++ -Wextra -Wsign-conversion")

add_custom_command(TARGET ve2
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/src/renderers/demo_renderer/demo.vert ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/src/renderers/demo_renderer/demo.frag ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/src/renderers/coordinate_lines_renderer/coordinate_lines.vert ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/src/renderers/coordinate_lines_renderer/coordinate_lines.frag ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/src/assets/fonts/Inconsolata-Regular.ttf ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/src/assets/fonts/NotoSans-Bold.ttf ${PROJECT_BINARY_DIR}
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/src/assets/fonts/NotoSans-Regular.ttf ${PROJECT_BINARY_DIR}
)

target_link_libraries(ve2 ${CONAN_LIBS})
