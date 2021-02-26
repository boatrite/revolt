#include "main_menu_bar.h"

MainMenuBar::MainMenuBar(const UIContext& ui_context) : m_ui_context {ui_context} {
}

void MainMenuBar::show() {
  if (ImGui::BeginMainMenuBar()) {
    auto main_menu_bar_size = ImGui::GetWindowSize();
    if (ImGui::BeginMenu("File")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
      if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {} // Disabled item
      ImGui::Separator();
      if (ImGui::MenuItem("Cut", "CTRL+X")) {}
      if (ImGui::MenuItem("Copy", "CTRL+C")) {}
      if (ImGui::MenuItem("Paste", "CTRL+V")) {}
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
    glViewport(0, 0, m_ui_context.getWidth(), m_ui_context.getHeight() - main_menu_bar_size.y);
  }
}
