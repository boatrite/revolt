#include <imgui.h>

#include <algorithm>

#include "ui_context.h"

UIContext::UIContext(GLFWwindow* window) : m_window{window} {
  // TODO Eventually I want these callbacks all defined here I think? All the UI
  // and input stuff.
  // glfwSetWindowSizeCallback(m_window, windowSizeCallback);
}

int UIContext::getWidth() const {
  return m_width;
}

int UIContext::getHeight() const {
  return m_height;
}

//
// General purpose handler functionality
//
// void UIContext::removeHandlers(void* instance) {
  // removeCursorMovedHandler(instance);
// }

//
// Cursor Moved Callback
//
void UIContext::cursorPosCallback(double xpos, double ypos) {
  auto it = m_cursor_moved_handlers.rbegin();
  if (it != m_cursor_moved_handlers.rend()) {
    it->second(xpos, ypos);
  }
}

void UIContext::addCursorMovedHandler(void* instance, std::function<void(double, double)> f) {
  m_cursor_moved_handlers.insert(std::make_pair(instance, f));
}

void UIContext::removeCursorMovedHandler(void* instance) {
  m_cursor_moved_handlers.erase(instance);
}

//
// Key Callback
//
void UIContext::keyCallback(int key, int scancode, int action, int mods) {
  switch (action) {
    case GLFW_PRESS:
      {
        // Get all callbacks for a specific key. The current methodology is that
        // we only invoke the latest callback for a specific key.
        auto it = m_handlers_by_key_map.find(key);
        if (it != m_handlers_by_key_map.end()) { // If we have anything bound for this key
          auto& handlers = it->second;
          auto it2 = handlers.rbegin();
          if (it2 != handlers.rend()) {
            it2->second();
          }
        }
        break;
      }
    case GLFW_REPEAT:
      {
        break;
      }
    case GLFW_RELEASE:
      {
        break;
      }
  }
}

int UIContext::addKeyPressedHandler(int key, std::function<void()> f) {
  static int curr_id { 1 };
  curr_id++;

  auto it = m_handlers_by_key_map.find(key);
  if (it == m_handlers_by_key_map.end()) {
    std::map<int, std::function<void()>> handlers {}; // Create new empty map
    handlers.insert(std::make_pair(curr_id, f)); // Add our handler to it
    m_handlers_by_key_map.insert(std::make_pair(key, handlers)); // Add our new handlers for the key
  } else {
    auto& handlers = it->second;
    handlers.insert(std::make_pair(curr_id, f));
  }

  return curr_id;
}

void UIContext::removeKeyPressedHandler(int handler_id) {
  for (auto& [key, handlers] : m_handlers_by_key_map) {
    handlers.erase(handler_id);
  }
}

//
// Key State
//
bool UIContext::isKeyPressed(int key) {
  return glfwGetKey(m_window, key) == GLFW_PRESS;
}

//
//
//
bool UIContext::isFocusedInGame() const {
  // We only want to process in-game input when we are actually "in the game".
  //
  // Now that we have a GUI we want to interact with, we need to know whether
  // we are focused in game or focused in the gui, and only do input for one or
  // the other.
  int cursorMode = glfwGetInputMode(m_window, GLFW_CURSOR);
  return cursorMode == GLFW_CURSOR_DISABLED;
}

bool UIContext::isFocusedInGUI() const {
  return !isFocusedInGame();
}

void UIContext::focusInGame() {
  if (!isFocusedInGame()) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide cursor

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;            // Disable Mouse
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard; // Disable Keyboard

    // App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    // app_ptr->focusCallback(true); // focus == true -> focused in game
  }
}

void UIContext::focusInGUI() {
  if (!isFocusedInGUI()) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show cursor

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;          // Enable Mouse
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard

    // App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    // app_ptr->focusCallback(false); // focus == false -> focused in GUI
  }
}
