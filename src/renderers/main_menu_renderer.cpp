#include "../font_book.h"
#include "main_menu_renderer.h"

MainMenuRenderer::MainMenuRenderer(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
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
  ImGui::SetNextWindowSize(ImVec2(m_ui_context_ptr->getWidth(), m_ui_context_ptr->getHeight()));
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

  ImVec2 windowSize = ImGui::GetIO().DisplaySize;
  ImVec2 buttonSize(280, 40);
  ImGui::SetCursorPos(ImVec2(windowSize.x/2 - buttonSize.x/2, windowSize.y/2 - buttonSize.y/2)); // Move cursor on needed positions
  ImGui::PushFont(FontBook::NotoSans28Bold());
  if (ImGui::Button("New World", buttonSize)) {
    std::cout << "New world button clicked." << std::endl;
  }
  ImGui::PopFont();

  ImGui::End();

  // Revert to old style
  style = *previous_ptr;
}
