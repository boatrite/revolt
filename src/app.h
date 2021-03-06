#pragma once

#include "rendering/main_menu_renderer.h"
#include "rendering/root_renderer.h"
#include "ui_context.h"

class App {
 private:
  std::vector<int> m_handler_ids {};
  std::shared_ptr<UIContext> m_ui_context_ptr;
  bool m_show_demo_window {false};

  void ShowMainMenuBar();

 public:
  App(std::shared_ptr<UIContext> ui_context_ptr);
  ~App();
  void update(double dt);
  void render(double dt);
};
