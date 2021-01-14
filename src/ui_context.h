# pragma once

#include <functional>
#include <iostream>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class UIContext {
  private:
    GLFWwindow* m_window;
    int m_width;
    int m_height;

  public:
    UIContext(GLFWwindow* window);

    //
    //
    //
    int getWidth() const;
    int getHeight() const;

    //
    //
    //
    bool isFocusedInGame() const;
    bool isFocusedInGUI() const;
    void focusInGame();
    void focusInGUI();

    //
    // Key Callback
    //
  private:
    // std::map<int, std::vector<const std::function<void()>&>> m_map;
    std::vector<std::function<void()>> m_callbacks {
      []() { std::cout << "hello from f12 key pressed callback" << std::endl; }
    };

  public:
    // Called by Window
    void keyCallback(int key, int scancode, int action, int mods);

    // Called by consumers of UIContext
    int pushKeyHandler(int key, std::function<void()> f);
    void popKeyHandler(int handler_id);
};
