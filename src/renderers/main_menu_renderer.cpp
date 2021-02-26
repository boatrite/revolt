#include "main_menu_renderer.h"

#include "../actions/change_current_page_action.h"
#include "../font_book.h"
#include "../util/imgui_util.h"
#include "new_world_page_renderer.h"
#include "root_renderer.h"

MainMenuRenderer::MainMenuRenderer(std::shared_ptr<UIContext> ui_context_ptr) :
  m_ui_context_ptr {ui_context_ptr} {
  std::cout << "MainMenuRenderer (" << this << ") created" << std::endl;

  // Create custom style for rendering window.
  m_style = ImGuiUtil::CreateStyle([](ImGuiStyle& style) {
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
  });

  m_ui_context_ptr->focusInGUI();
}

MainMenuRenderer::~MainMenuRenderer() {
  std::cout << "MainMenuRenderer (" << this << ") destroyed" << std::endl;
}

void MainMenuRenderer::render(double dt) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGuiUtil::PushStyle(m_style);
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImVec2 windowSize = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowSize(windowSize);
  // ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGui::Begin("Main Menu",
               NULL,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
                 | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings);
  ImVec2 buttonSize(280, 40);
  ImGui::SetCursorPos(
    ImVec2(windowSize.x / 2 - buttonSize.x / 2,
           windowSize.y / 2 - buttonSize.y / 2)); // Move cursor on needed positions
  ImGui::PushFont(FontBook::NotoSans28Bold());
  if (ImGui::Button("New World", buttonSize)) {
    m_ui_context_ptr->getStore().dispatch(
      ChangeCurrentPageAction(std::make_shared<NewWorldPageRenderer>(m_ui_context_ptr)));
  }
  ImGui::SetCursorPos(ImVec2(windowSize.x / 2 - buttonSize.x / 2, ImGui::GetCursorPos().y + 10));
  if (ImGui::Button("Test", buttonSize)) {
    m_ui_context_ptr->getStore().dispatch(
      ChangeCurrentPageAction(std::make_shared<RootRenderer>(m_ui_context_ptr)));
  }
  ImGui::PopFont();
  ImGui::End();
  ImGuiUtil::PopStyle();
}
