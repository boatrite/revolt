#include "main_menu_renderer.h"

MainMenuRenderer::MainMenuRenderer() {
  // Duplicate the current imgui state
  const ImGuiStyle& style = ImGui::GetStyle();
  ref_saved_style = style;
  ref_saved_style_ptr = &ref_saved_style;

  // Apply edits to style
  ref_saved_style.WindowRounding = 0.0f;
  ref_saved_style.WindowBorderSize = 0.0f;
}

void MainMenuRenderer::render(double dt) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui::SetNextWindowPos(ImVec2(0, 0));
  // ImGui::SetNextWindowSize(ImVec2(100.0f, 100.0f));
  // ImGui::SetNextWindowSizeConstraints();
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

  const auto flags =
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoSavedSettings;

  // Backup current style
  ImGuiStyle& style = ImGui::GetStyle();
  ImGuiStyle previous = style;
  ImGuiStyle* previous_ptr = &previous;

  // Enable custom style
  style = *ref_saved_style_ptr;

  // Render
  ImGui::Begin("Main Menu", NULL, flags);
  if (ImGui::Button("New World")) {
  }
  // ImGui::ShowStyleEditor();
  ImGui::End();

  // Revert to old style
  style = *previous_ptr;
}
