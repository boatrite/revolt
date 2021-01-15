# pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Renderer {
  public:
    virtual ~Renderer() {}
    Renderer(const Renderer&) = delete; // Delete copy constructor
    Renderer& operator=(const Renderer&) = delete; // Delete copy assignment

    virtual void processInput(GLFWwindow* window, float dt) {};
    virtual void update(double dt) {};
    virtual void render(double dt) {};
  protected:
    Renderer() {}
};
