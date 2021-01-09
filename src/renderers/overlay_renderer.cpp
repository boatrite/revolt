#include <imgui.h>

#include "overlay_renderer.h"

OverlayRenderer::OverlayRenderer(std::shared_ptr<Camera> cameraPtr) : m_cameraPtr { cameraPtr } {
}

void OverlayRenderer::render(double dt) {
  ImGuiIO& io = ImGui::GetIO();

  const float DISTANCE = 10.0f;
  ImVec2 window_pos = ImVec2(DISTANCE, DISTANCE);
  ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

  const auto flags =
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoSavedSettings |
    ImGuiWindowFlags_NoFocusOnAppearing |
    ImGuiWindowFlags_NoNav;

  if (ImGui::Begin("Info", &m_show_overlay, flags)) {
    ImGui::Text("Debug Info");

    ImGui::Separator();

    ImGui::Text("Mouse:");
    if (ImGui::IsMousePosValid()) {
      ImGui::Text("(%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
    } else {
      ImGui::Text("<invalid>");
    }
  }
}
