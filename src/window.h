#pragma once

#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window {
  public:
    Window(std::string title, int width, int height);
    int show();

  private:
    std::string m_title;
    int m_width;
    int m_height;
    GLFWwindow* m_window;

    static void GLAPIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void windowSizeCallback(GLFWwindow* window, int width, int height);

    static bool isFocusedInGame(GLFWwindow* window);
    static bool isFocusedInGUI(GLFWwindow* window);
    static void focusInGame(GLFWwindow* window);
    static void focusInGUI(GLFWwindow* window);
};
