#pragma once

#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ui_context.h"

class Window {
  public:
    Window(std::string title, int width, int height);
    int show();

  private:
    std::string m_title;
    int m_width;
    int m_height;
    GLFWwindow* m_window;

    static std::shared_ptr<UIContext> s_ui_context_ptr;

    static void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
};
