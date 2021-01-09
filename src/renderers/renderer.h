# pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
  public:
    Renderer() {}
    virtual ~Renderer() {}
    virtual void processInput(GLFWwindow* window, float dt) {};
    virtual void update(double dt) {};
    virtual void render(double dt) {};

    virtual void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {};
    virtual void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {};
    virtual void windowSizeCallback(GLFWwindow* window, int width, int height) {};
    virtual void focusCallback(bool focusedInGame) {};
};
