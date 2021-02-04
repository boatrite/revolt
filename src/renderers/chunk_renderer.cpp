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
