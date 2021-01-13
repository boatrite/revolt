# pragma once

#include "renderers/root_renderer.h"
#include "renderers/main_menu_renderer.h"

class App {
  private:
    std::shared_ptr<Renderer> m_root_renderer_ptr { std::make_shared<MainMenuRenderer>() };

  public:
    void update(double dt);
    void render(double dt);

    void processInput(GLFWwindow* window, float dt);

    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void windowSizeCallback(GLFWwindow* window, int width, int height);
    void focusCallback(bool focusedInGame);
};
