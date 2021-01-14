#include <imgui.h>

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
// Key Callback
//
void UIContext::keyCallback(int key, int scancode, int action, int mods) {
  switch (action) {
    case GLFW_PRESS:
      // Get all callbacks for a specific key.
      // auto callbacks_for_key = m_map[key];
      // The current methodology is that we only invoke the latest callback for a
      // specific key.
      // callbacks_for_key[callbacks_for_key.size()-1]();
      m_callbacks[m_callbacks.size()-1]();
      break;
    case GLFW_REPEAT:
      break;
    case GLFW_RELEASE:
      break;
  }
}

int UIContext::pushKeyHandler(int key, const std::function<void()> f) {
  // auto callbacks_for_key = m_map[key];
  // callbacks_for_key.push_back(f);

  m_callbacks.push_back(f);
  return 1;
}

void UIContext::popKeyHandler(int handler_id) {
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
