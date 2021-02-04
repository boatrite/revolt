#include "../../extern/imgui_stdlib.h"

#include "../font_book.h"
#include "../util/imgui_util.h"
#include "root_renderer.h"
#include "new_world_page_renderer.h"

NewWorldPageRenderer::NewWorldPageRenderer(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  std::cout << "NewWorldPageRenderer (" << this << ") created" << std::endl;

  // Create custom style for rendering window.
  m_style = ImGuiUtil::CreateStyle(
    [](ImGuiStyle& style) {
      style.WindowRounding = 0.0f;
      style.WindowBorderSize = 0.0f;
    }
  );
}

NewWorldPageRenderer::~NewWorldPageRenderer() {
  std::cout << "NewWorldPageRenderer (" << this << ") destroyed" << std::endl;
}

void NewWorldPageRenderer::render(double dt) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGuiUtil::PushStyle(m_style);
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImVec2 windowSize = ImGui::GetIO().DisplaySize;
  ImGui::SetNextWindowSize(windowSize);
  // ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
  ImGui::Begin("Main Menu", NULL,
      ImGuiWindowFlags_NoMove |
      ImGuiWindowFlags_NoDecoration |
      ImGuiWindowFlags_AlwaysAutoResize |
      ImGuiWindowFlags_NoSavedSettings);
  ImVec2 buttonSize(280, 40);
  ImGui::SetCursorPos(ImVec2(windowSize.x/2 - buttonSize.x/2, 20)); // Move cursor on needed positions
  ImGui::PushFont(FontBook::NotoSans28Bold());
  ImGui::InputText("Seed", &m_world_seed, 0);
  ImGui::SetCursorPos(ImVec2(windowSize.x/2 - buttonSize.x/2, ImGui::GetCursorPos().y+10));
  if (ImGui::Button("Create World", buttonSize)) {
    std::cout << "Create world button clicked. Seed: " + m_world_seed << std::endl;
    m_ui_context_ptr->getStore().dispatch(CreateNewWorldAction(m_world_seed));
    m_ui_context_ptr->changeCurrentPage(
      std::make_shared<RootRenderer>(m_ui_context_ptr)
    );
  }
  ImGui::PopFont();
  ImGui::End();
  ImGuiUtil::PopStyle();
}
