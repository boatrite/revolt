# pragma once

#include <functional>
#include <iostream>
#include <map>

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
    // General purpose handler functionality
    //
  // public:
    // void removeHandlers(void* instance);

    //
    // Cursor Moved Callback
    //
  private:
    std::map<void*, std::function<void(double,double)>> m_cursor_moved_handlers;
  public:
    // Called by window
    void cursorPosCallback(double xpos, double ypos);

    // Called by consumers
    void addCursorMovedHandler(void* instance, std::function<void(double, double)> f);
    void removeCursorMovedHandler(void* instance);

    //
    // Key Callback
    //
  private:
    std::map<int, std::map<int,std::function<void()>>> m_handlers_by_key_map;

  public:
    // Called by Window
    void keyCallback(int key, int scancode, int action, int mods);

    // Called by consumers of UIContext
    int addKeyPressedHandler(int key, std::function<void()> f);
    void removeKeyPressedHandler(int handler_id);

    //
    // Key State
    //
  public:
    bool isKeyPressed(int key);
};
