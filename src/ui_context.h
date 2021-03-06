#pragma once

#include "redux.hpp"
#include "rendering/renderer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <entt/entt.hpp>

#include <functional>
#include <iostream>
#include <map>
#include <memory>

class UIContext {
 private:
  GLFWwindow* m_window;

 public:
  UIContext(GLFWwindow* window);
  ~UIContext();

  //
  // Well, I'm going to try to use redux again.
  // I'm just putting it here because it's the easiest way to be
  // able to access it since most places have a ui context.
  //
 private:
  using Store = redux::Store<entt::registry, std::function<void(entt::registry&)>>;
  Store m_store {Store()};

 public:
  Store& getStore() {
    return m_store;
  };
  entt::registry& getRegistry() {
    return getStore().getState();
  };

  //
  // Quit functionality
  //
 public:
  void quit();

  //
  // Window size accessors.
  // Gives consumers the ability to get the width and height each frame
  // without having to query glfw or deal with a callback and track the
  // values itself. UIContext uses the callback to keep them up-to-date.
  //
 private:
  int m_width;
  int m_height;
 public:
  void windowSizeCallback(int width, int height);
  int getWidth() const;
  int getHeight() const;

  //
  // General purpose handler functionality
  //
 public:
  void removeHandlers(void* instance);

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
  // Cursor Moved Callback
  //
 private:
  std::map<void*, std::function<void(double, double)>> m_cursor_moved_handlers;
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
  std::map<int, std::map<void*, std::function<void()>>> m_handlers_by_key_map;
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
  void focusInGame();
  void focusInGUI();
  bool isFocusedInGame() const;
  bool isFocusedInGUI() const;
  void addGameGuiFocusChangedHandler(void* instance, std::function<void(bool)>);
};
