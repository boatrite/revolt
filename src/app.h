# pragma once

#include "redux.hpp"

#include "actions.h"
#include "ui_context.h"
#include "renderers/root_renderer.h"
#include "renderers/main_menu_renderer.h"

class App {
  private:
    std::vector<int> m_handler_ids {};
    std::shared_ptr<UIContext> m_ui_context_ptr;
    std::shared_ptr<AppStore> m_store_ptr {
      std::make_shared<AppStore>(
        appStateReducer,
        std::make_shared<AppState>(m_ui_context_ptr)
      )
    };
    bool m_show_demo_window { false };

  public:
    App(std::shared_ptr<UIContext> ui_context_ptr);
    ~App();
    void update(double dt);
    void render(double dt);
};
