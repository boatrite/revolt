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
  m_shader.use();
  m_shader.setMat4("view", m_camera_ptr->getViewMatrix());
  m_shader.setMat4("projection", m_camera_ptr->getProjectionMatrix());

  const auto& e = m_ui_context_ptr->getRegistry().view<World>().front();
  const auto& world = m_ui_context_ptr->getRegistry().get<World>(e);
  for (const auto& chunk_ptr : world.chunks) {
    if (chunk_ptr) {
      chunk_ptr->render(m_shader);
    }
  }
}
