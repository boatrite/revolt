#include "app.h"

App::App(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  m_ui_context_ptr->changeCurrentPage(
    std::make_shared<MainMenuRenderer>(m_ui_context_ptr)
  );

  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_ESCAPE,
    this,
    [=]() { m_ui_context_ptr->quit(); }
  );

  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_F12,
    this,
    [=]() {
      m_show_demo_window = !m_show_demo_window;
    }
  );

  m_ui_context_ptr->getStore().dispatch(std::make_shared<CreateNewWorldAction>("foobar"));
  m_ui_context_ptr->changeCurrentPage(
    std::make_shared<RootRenderer>(m_ui_context_ptr)
  );
}

App::~App() {
  m_ui_context_ptr->removeHandlers(this);
}

void App::update(double dt) {
}

void App::render(double dt) {
  if (m_ui_context_ptr->getCurrentPagePtr()) {
    m_ui_context_ptr->getCurrentPagePtr()->render(dt);
  }

  if (m_show_demo_window) {
    ImGui::ShowDemoWindow();
  }
}
