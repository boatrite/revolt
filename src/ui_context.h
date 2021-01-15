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
    // Window size accessors.
    // Uses the callback to keep them up-to-date.
    //
  public:
    void windowSizeCallback(int width, int height);
    int getWidth() const;
    int getHeight() const;

    //
    // Per-frame Input processing
    //
  private:
    std::map<void*, std::function<void(double)>> m_process_input_funcs;
    void removeProcessInput(void* instance);
  public:
    void processInput(double dt);
    void addProcessInput(void* instance, std::function<void(double)> f);

    //
    // General purpose handler functionality
    //
  public:
    void removeHandlers(void* instance);

    //
    // Cursor Moved Callback
    //
  private:
    std::map<void*, std::function<void(double,double)>> m_cursor_moved_handlers;
    void removeCursorMovedHandler(void* instance);
  public:
    // Called by window
    void cursorPosCallback(double xpos, double ypos);

    // Called by consumers
    void addCursorMovedHandler(void* instance, std::function<void(double, double)> f);

    //
    // Key Callback
    //
  private:
    std::map<int, std::map<void*,std::function<void()>>> m_handlers_by_key_map;
    void removeKeyPressedHandlers(void* instance);

  public:
    // Called by Window
    void keyCallback(int key, int scancode, int action, int mods);

    // Called by consumers of UIContext
    void addKeyPressedHandler(int key, void* instance, std::function<void()> f);

    //
    // Key State
    //
  public:
    bool isKeyPressed(int key);

    //
    // Game/Gui Focus
    //
  private:
    std::map<void*, std::function<void(bool)>> m_game_gui_focus_changed_handlers;
    void removeGameGuiFocusChangedHandler(void* instance);
  public:
    bool isFocusedInGame() const;
    bool isFocusedInGUI() const;
    void focusInGame();
    void focusInGUI();
    void addGameGuiFocusChangedHandler(void* instance, std::function<void(bool)>);
};
