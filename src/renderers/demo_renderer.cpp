#include <iostream>
#include <vector>
#include "demo_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

DemoRenderer::DemoRenderer(std::shared_ptr<Camera> cameraPtr) : m_cameraPtr { cameraPtr } {
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

  auto grass_cube = color_cube(Color::grass);
  auto mesh = grass_cube;
  glBufferData(GL_ARRAY_BUFFER, static_cast<float>(mesh.size()) * sizeof(float), mesh.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, m_floats_per_vertex * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, m_floats_per_vertex * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

DemoRenderer::~DemoRenderer() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void DemoRenderer::render(double dt) {
  glEnable(GL_DEPTH_TEST);

  glClearColor(0.132f, 0.132f, 0.132f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shader.use();
  m_shader.setMat4("view", m_cameraPtr->getViewMatrix());
  m_shader.setMat4("projection", m_cameraPtr->getProjectionMatrix());

  glBindVertexArray(m_vao);

  for (auto i = 0; i <= 8 * (1.0 / m_scale); ++i) {
    for (auto ii = 0; ii <= 8 * (1.0 / m_scale); ++ii) {
      m_shader.setMat4(
        "model",
        glm::translate( // Move it to (i, ii) in the fake chunk thing
          glm::scale( // Scale the unit cube to create our fundamental building block
            glm::mat4(1.0f),
            glm::vec3(m_scale)
          ),
          glm::vec3(i+0.5f, 0.5f, ii+0.5f) // Offset by 0.5f to align on integer values.
        )
      );
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }
  }

  glBindVertexArray(0);
}

std::array<float, 36 * 6> DemoRenderer::color_cube(Color color_code) {
  static glm::vec3 grass = glm::vec3(34.0 / 255.0, 70.0 / 255.0, 29.0 / 255.0);
  static glm::vec3 water = glm::vec3(2.0 / 255.0, 43.0 / 255.0, 61.0 / 255.0);
  static glm::vec3 dirt = glm::vec3(38.0 / 255.0, 28.0 / 255.0, 20.0 / 255.0);
  static glm::vec3 sand = glm::vec3(198.0 / 255.0, 176.0 / 255.0, 128.0 / 255.0);

  glm::vec3 color;
  switch (color_code) {
    case Color::grass:
      color = grass;
      break;
    case Color::dirt:
      color = dirt;
      break;
    case Color::sand:
      color = sand;
      break;
    case Color::water:
      color = water;
      break;
  }

  std::array<float, 36 * 6> vertices {};

  for (auto i = 0; i < m_cube_vertices.size(); i += 3) {
    auto ii = i*2;
    vertices[ii] = m_cube_vertices[i];
    vertices[ii+1] = m_cube_vertices[i+1];
    vertices[ii+2] = m_cube_vertices[i+2];

    vertices[ii+3] = color.x;
    vertices[ii+4] = color.y;
    vertices[ii+5] = color.z;
  }

  return vertices;
}
