#include "app.h"

App::App(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  m_store_ptr->getState()->ui_context_ptr->focusInGUI();

  m_store_ptr->dispatch(Action::setRenderer(std::make_shared<MainMenuRenderer>(m_store_ptr, m_ui_context_ptr)));

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
  if (m_store_ptr->getState()->renderer_ptr) {
    m_store_ptr->getState()->renderer_ptr->render(dt);
  }

  if (m_show_demo_window) {
    ImGui::ShowDemoWindow();
  }
}
