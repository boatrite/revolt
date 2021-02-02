#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include "../common.h"
#include "chunk_renderer.h"

ChunkRenderer::ChunkRenderer(std::shared_ptr<UIContext> ui_context_ptr, std::shared_ptr<Camera> camera_ptr) : m_ui_context_ptr{ui_context_ptr}, m_camera_ptr {camera_ptr} {}

void ChunkRenderer::render(double dt) {
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.132f, 0.132f, 0.132f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shader.use();
  m_shader.setMat4("view", m_camera_ptr->getViewMatrix());
  m_shader.setMat4("projection", m_camera_ptr->getProjectionMatrix());

  for (const auto& chunk_ptr : m_ui_context_ptr->getStore().getState().chunks) {
    if (chunk_ptr) {
      chunk_ptr->render(m_shader);
    }
  }
}

// std::array<float, 36 * 6> ChunkRenderer::color_cube(Color color_code) {
  // static glm::vec3 grass = glm::vec3(34.0 / 255.0, 70.0 / 255.0, 29.0 / 255.0);
  // static glm::vec3 water = glm::vec3(2.0 / 255.0, 43.0 / 255.0, 61.0 / 255.0);
  // static glm::vec3 dirt = glm::vec3(38.0 / 255.0, 28.0 / 255.0, 20.0 / 255.0);
  // static glm::vec3 sand = glm::vec3(198.0 / 255.0, 176.0 / 255.0, 128.0 / 255.0);

  // glm::vec3 color;
  // switch (color_code) {
    // case Color::grass:
      // color = grass;
      // break;
    // case Color::dirt:
      // color = dirt;
      // break;
    // case Color::sand:
      // color = sand;
      // break;
    // case Color::water:
      // color = water;
      // break;
  // }

  // std::array<float, 36 * 6> vertices {};

  // for (auto i = 0; i < m_cube_vertices.size(); i += 3) {
    // auto ii = i*2;
    // vertices[ii] = m_cube_vertices[i];
    // vertices[ii+1] = m_cube_vertices[i+1];
    // vertices[ii+2] = m_cube_vertices[i+2];

    // vertices[ii+3] = color.x;
    // vertices[ii+4] = color.y;
    // vertices[ii+5] = color.z;
  // }

  // return vertices;
// }
