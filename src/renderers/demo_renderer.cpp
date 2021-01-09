#include <iostream>
#include "demo_renderer.h"

DemoRenderer::DemoRenderer(std::shared_ptr<Camera> cameraPtr) : m_cameraPtr { cameraPtr } {
  glGenVertexArrays(1, &m_vao);
  glGenBuffers(1, &m_vbo);

  int floats_per_vertex = 6;
  float vertices[] = {
    // x, y, z, r, g, b
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.0f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f
  };

  glBindVertexArray(m_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, floats_per_vertex * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, floats_per_vertex * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

DemoRenderer::~DemoRenderer() {
  glDeleteBuffers(1, &m_vbo);
  glDeleteVertexArrays(1, &m_vao);
}

void DemoRenderer::render(double dt) {
  glClearColor(0.132f, 0.132f, 0.132f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  m_shader.use();

  m_shader.setMat4("model", glm::mat4(1.0f));
  m_shader.setMat4("view", m_cameraPtr->getViewMatrix());
  m_shader.setMat4("projection", m_cameraPtr->getProjectionMatrix());

  glBindVertexArray(m_vao);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glBindVertexArray(0);
}
