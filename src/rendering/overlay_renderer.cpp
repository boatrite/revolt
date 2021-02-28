#include "overlay_renderer.h"

#include "../util/imgui_helper.h"

#include <imgui.h>

OverlayRenderer::OverlayRenderer(std::shared_ptr<Camera> cameraPtr) : m_cameraPtr {cameraPtr} {
}

void OverlayRenderer::render(double dt) {
  ImGuiIO& io = ImGui::GetIO();

  const auto flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
                   | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings
                   | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

  ImGuiHelper::PushNextWindowIntoLeftColumn();
  if (ImGui::Begin("Info", &m_show_overlay, flags)) {
    ImGui::Text("Debug Info");

    ImGui::Separator();

    ImGui::Text("Mouse:");
    if (ImGui::IsMousePosValid()) {
      ImGui::Text("(%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
    } else {
      ImGui::Text("<invalid>");
    }

    ImGui::Separator();

    m_cameraPtr->imguiDebugControlPanel();
  }
}
