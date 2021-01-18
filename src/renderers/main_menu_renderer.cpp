#include "../font_book.h"
#include "../util/imgui_util.h"
#include "main_menu_renderer.h"
#include "root_renderer.h"

MainMenuRenderer::MainMenuRenderer(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  std::cout << "MainMenuRenderer (" << this << ") created" << std::endl;

  // Create custom style for rendering window.
  m_style = ImGuiUtil::CreateStyle(
    [](ImGuiStyle& style) {
      style.WindowRounding = 0.0f;
      style.WindowBorderSize = 0.0f;
    }
  );

  m_ui_context_ptr->focusInGUI();
}

MainMenuRenderer::~MainMenuRenderer() {
  std::cout << "MainMenuRenderer (" << this << ") destroyed" << std::endl;
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

  ImGuiUtil::PushStyle(m_style);

  // Render
  ImGui::Begin("Main Menu", NULL, flags);
  ImVec2 windowSize = ImGui::GetIO().DisplaySize;
  ImVec2 buttonSize(280, 40);
  ImGui::SetCursorPos(ImVec2(windowSize.x/2 - buttonSize.x/2, windowSize.y/2 - buttonSize.y/2)); // Move cursor on needed positions
  ImGui::PushFont(FontBook::NotoSans28Bold());
  if (ImGui::Button("New World", buttonSize)) {
    std::cout << "New world button clicked." << std::endl;
  }
  ImGui::SetCursorPos(ImVec2(windowSize.x/2 - buttonSize.x/2, ImGui::GetCursorPos().y+10));
  if (ImGui::Button("Test", buttonSize)) {
    std::cout << "Test button clicked." << std::endl;
    m_ui_context_ptr->changeCurrentPage(
      std::make_shared<RootRenderer>(m_ui_context_ptr)
    );
  }
  ImGui::PopFont();
  ImGui::End();

  ImGuiUtil::PopStyle();
}
