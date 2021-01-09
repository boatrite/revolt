#include <glad/glad.h>

#include "root_renderer.h"

RootRenderer::RootRenderer() {
};

void RootRenderer::render(double dt) {
  glClearColor(0.132f, 0.132f, 0.132f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
  };

  if (!m_vao) {
    glGenVertexArrays(1, &m_vao);
  }

  if (!m_vbo) {
    glGenBuffers(1, &m_vbo);
  }

  if (m_dirty) {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_dirty = false;
  } else {
    m_shader.use();
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  glBindVertexArray(0);
}
