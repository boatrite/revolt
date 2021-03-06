#include "app.h"

#include "actions/change_current_page_action.h"
#include "actions/create_new_world_action.h"
#include "hud/main_menu_bar.h"

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
  const UIContext& ui_context = *m_ui_context_ptr;
  static MainMenuBar s_main_menu_bar {MainMenuBar(ui_context)};
  s_main_menu_bar.show();

  const auto& current_page = m_ui_context_ptr->getRegistry().ctx<CurrentPage>();
  if (current_page.renderer_ptr) {
    current_page.renderer_ptr->render(dt);
  }

  if (m_show_demo_window) {
    ImGui::ShowDemoWindow();
  }
}
