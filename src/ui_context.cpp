#include "ui_context.h"

#include <imgui.h>
#include "bindings/imgui_impl_glfw.h"

#include <algorithm>

UIContext::UIContext(GLFWwindow* window) : m_window{window} {
  glfwSetWindowUserPointer(window, this);

  glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
    UIContext* ui_context_ptr = static_cast<UIContext*>(glfwGetWindowUserPointer(window));
    ui_context_ptr->windowSizeCallback(width, height);
  });

  glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos) {
    UIContext* ui_context_ptr = static_cast<UIContext*>(glfwGetWindowUserPointer(window));
    ui_context_ptr->cursorPosCallback(xpos, ypos);
  });

  glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    UIContext* ui_context_ptr = static_cast<UIContext*>(glfwGetWindowUserPointer(window));
    ui_context_ptr->keyCallback(key, scancode, action, mods);
  });

  glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
    // Forward to Dear Imgui so scroll behavior works in imgui windows.
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);

    // Not yet implemented in UIContext
    // std::cout << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
  });
}

UIContext::~UIContext() {
  std::cout << "~UIContext() called" << std::endl;
}

//
// Quit functionality
//
void UIContext::quit() {
  std::cout << "UIContext::quit() called. Telling glfw window to close now." << std::endl;
  glfwSetWindowShouldClose(m_window, 1);
}

//
// Window size accessors.
// Uses the callback to keep them up-to-date.
//
void UIContext::windowSizeCallback(int width, int height) {
  m_width = width;
  m_height = height;
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
void UIContext::removeHandlers(void* instance) {
  std::cout << "Removing handlers for instance " << instance << std::endl;
  removeCursorMovedHandler(instance);
  removeGameGuiFocusChangedHandler(instance);
  removeKeyPressedHandlers(instance);
  removeProcessInput(instance);
}

//
// Per-frame Input processing
//
void UIContext::processInput(double dt) {
  for (auto& [instance, f] : m_process_input_funcs) {
    f(dt);
  }
}

void UIContext::addProcessInput(void* instance, std::function<void(double)> f) {
  m_process_input_funcs.insert(std::make_pair(instance, f));
}

void UIContext::removeProcessInput(void* instance) {
  m_process_input_funcs.erase(instance);
}

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
        // FIXME This doesn't work exactly right, I used to store an int as the
        // key, so the ordered map would have the latest one last. Now I store a
        // void* pointer, I might need to store a timestamp too now or something. I
        // don't have any bindings where this matters right now, but I was
        // thinking ESC would be one
        // that could be a test case for rebinding, as in when closing
        // nested interfaces.
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

void UIContext::addKeyPressedHandler(int key, void* instance, std::function<void()> f) {
  auto it = m_handlers_by_key_map.find(key);
  if (it == m_handlers_by_key_map.end()) {
    std::map<void*, std::function<void()>> handlers {}; // Create new empty map
    handlers.insert(std::make_pair(instance, f)); // Add our handler to it
    m_handlers_by_key_map.insert(std::make_pair(key, handlers)); // Add our new handlers for the key
  } else {
    auto& handlers = it->second;
    handlers.insert(std::make_pair(instance, f));
  }
}

void UIContext::removeKeyPressedHandlers(void* instance) {
  for (auto& [key, handlers] : m_handlers_by_key_map) {
    handlers.erase(instance);
  }
}

//
// Key State
//
bool UIContext::isKeyPressed(int key) {
  return glfwGetKey(m_window, key) == GLFW_PRESS;
}

//
// Game/Gui Focus
//
bool UIContext::isFocusedInGame() const {
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

    for (auto& [instance, handler] : m_game_gui_focus_changed_handlers) {
      handler(true);
    }
  }
}

void UIContext::focusInGUI() {
  if (!isFocusedInGUI()) {
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show cursor

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;          // Enable Mouse
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard

    for (auto& [instance, handler] : m_game_gui_focus_changed_handlers) {
      handler(false);
    }
  }
}

void UIContext::addGameGuiFocusChangedHandler(void* instance, std::function<void(bool)> f) {
  m_game_gui_focus_changed_handlers.insert(std::make_pair(instance, f));
}

void UIContext::removeGameGuiFocusChangedHandler(void* instance) {
  m_game_gui_focus_changed_handlers.erase(instance);
}
