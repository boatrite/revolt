# pragma once

#include "renderers/root_renderer.h"

class App {
  private:
    std::shared_ptr<RootRenderer> m_root_renderer_ptr { std::make_shared<RootRenderer>() };

  public:
    void update(double dt);
    void render(double dt);

    void processInput(GLFWwindow* window, float dt);

    void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void windowSizeCallback(GLFWwindow* window, int width, int height);
    void focusCallback(bool focusedInGame);
};
