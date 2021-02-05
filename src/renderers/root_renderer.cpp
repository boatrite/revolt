#include <imgui.h>

#include "../util/imgui_util.h"

#include "../action.h"
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

  ImVec2 windowSize = ImGui::GetIO().DisplaySize;
  const int window_width = 300;
  ImGui::SetNextWindowPos(ImVec2(windowSize.x - 10 - window_width, 10));
  ImGui::SetNextWindowSize(ImVec2(window_width, 0));

  if (ImGui::Begin("State")) {
    State& state = m_ui_context_ptr->getStore().getState();

    ImGui::Text("world_seed: %s", state.world_ptr->seed.c_str());

    ImGui::Text("world_size: %ix%i", state.world_ptr->width_in_chunks, state.world_ptr->length_in_chunks);

    static auto scale_factor_slider =
      ImGuiUtil::SliderInt("scale_factor", &state.world_ptr->scale_factor, 0, 3, [=](auto prev, auto next) {
        // When the scale factor changes, we need to recreate the Chunk objects.
        m_ui_context_ptr->getStore().dispatch(RecreateChunksAction(next));
      });
    scale_factor_slider();

    ImGui::Text("scale: %.3f", state.world_ptr->scale());
  }
  ImGui::End();

  if (ImGui::Begin("Info")) {
    ImGui::Separator();
    ImGui::Checkbox("wireframe", &m_wireframe);
  }
}
