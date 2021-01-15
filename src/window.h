#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ui_context.h"

class Window {
  public:
    int show(std::string title, int width, int height);

  private:
    GLFWwindow* m_window;

    static std::shared_ptr<UIContext> s_ui_context_ptr;

    static void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
};
