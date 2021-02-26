#include "app.h"

App::App(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr {ui_context_ptr} {
  m_ui_context_ptr->getStore().dispatch(
    ChangeCurrentPageAction(std::make_shared<MainMenuRenderer>(m_ui_context_ptr)));

  m_ui_context_ptr->addKeyPressedHandler(GLFW_KEY_ESCAPE, this, [=]() {
    m_ui_context_ptr->quit();
  });

  m_ui_context_ptr->addKeyPressedHandler(GLFW_KEY_F12, this, [=]() {
    m_show_demo_window = !m_show_demo_window;
  });

  m_ui_context_ptr->getStore().dispatch(CreateNewWorldAction("foobar", 0));
  m_ui_context_ptr->getStore().dispatch(
    ChangeCurrentPageAction(std::make_shared<RootRenderer>(m_ui_context_ptr)));
}

App::~App() {
  std::cout << "~App() entered" << std::endl;
  m_ui_context_ptr->removeHandlers(this);
  std::cout << "~App() finished" << std::endl;
}

void App::update(double dt) {
}

void App::render(double dt) {
  const auto& current_page = m_ui_context_ptr->getRegistry().ctx<CurrentPage>();
  if (current_page.renderer_ptr) {
    current_page.renderer_ptr->render(dt);
  }

  if (m_show_demo_window) {
    ImGui::ShowDemoWindow();
  }
}
