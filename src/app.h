# pragma once

#include "ui_context.h"
#include "renderers/root_renderer.h"
#include "renderers/main_menu_renderer.h"

class App {
  private:
    std::vector<int> m_handler_ids {};
    std::shared_ptr<UIContext> m_ui_context_ptr;
    std::shared_ptr<Renderer> m_root_renderer_ptr { std::make_shared<RootRenderer>(m_ui_context_ptr) };
    bool m_show_demo_window { false };

  public:
    App(std::shared_ptr<UIContext> ui_context_ptr);
    ~App();
    void update(double dt);
    void render(double dt);

    void processInput(GLFWwindow* window, float dt);

    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void windowSizeCallback(GLFWwindow* window, int width, int height);
    void focusCallback(bool focusedInGame);
};