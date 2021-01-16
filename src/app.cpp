#include "app.h"

App::App(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  m_store_ptr->subscribe([](int state) { std::cout << state << std::endl; });

  m_ui_context_ptr->focusInGUI();

  ui_context_ptr->addKeyPressedHandler(
      GLFW_KEY_ESCAPE,
      this,
      [=]() { ui_context_ptr->quit(); }
  );

  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_F12,
    this,
    [=]() {
      m_show_demo_window = !m_show_demo_window;
    }
  );

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

}

App::~App() {
  m_ui_context_ptr->removeHandlers(this);
}

void App::update(double dt) {
}

void App::render(double dt) {
  m_root_renderer_ptr->render(dt);

  if (m_show_demo_window) {
    ImGui::ShowDemoWindow();
  }
}
