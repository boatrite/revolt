#include <imgui.h>

#include "root_renderer.h"
#include "chunk_renderer.h"
#include "overlay_renderer.h"
#include "coordinate_lines_renderer.h"
#include "chunk_boundaries_renderer.h"

RootRenderer::RootRenderer(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  std::cout << "RootRenderer (" << this << ") created" << std::endl;

  m_renderer_ptrs.push_back(std::make_shared<ChunkRenderer>(m_ui_context_ptr, m_camera_ptr));
  m_renderer_ptrs.push_back(std::make_shared<OverlayRenderer>(m_camera_ptr));
  m_renderer_ptrs.push_back(std::make_shared<CoordinateLinesRenderer>(m_camera_ptr));
  m_renderer_ptrs.push_back(std::make_shared<ChunkBoundariesRenderer>(m_ui_context_ptr, m_camera_ptr));

  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_E,
    this,
    [=]() {
      if (m_ui_context_ptr->isFocusedInGame()) {
        m_ui_context_ptr->focusInGUI();
      } else {
        m_ui_context_ptr->focusInGame();
      }
    }
  );

  m_ui_context_ptr->focusInGame();
}

RootRenderer::~RootRenderer() {
  m_ui_context_ptr->removeHandlers(this);
  std::cout << "RootRenderer (" << this << ") destroyed" << std::endl;
}

void RootRenderer::render(double dt) {
  if (m_wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  for(auto renderer_ptr : m_renderer_ptrs) {
    renderer_ptr->render(dt);
  }

  ImGui::SetNextWindowPos(ImVec2(10, 300));
  if (ImGui::Begin("State")) {
    const State& state = m_ui_context_ptr->getStore().getState();
    ImGui::Text("world_seed: %s", state.world_seed.c_str());
    ImGui::Text("world_size: %i", state.world_size);
  }
  ImGui::End();
}
