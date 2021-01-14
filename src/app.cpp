#include "app.h"

App::App(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  int id = m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_F12,
    [=]() {
      m_show_demo_window = !m_show_demo_window;
    }
  );
  m_handler_ids.push_back(id);

  id = m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_F11,
    []() {
      std::cout << "hello from f11 key pressed callback" << std::endl;
    }
  );
  m_handler_ids.push_back(id);
}

App::~App() {
  for (auto& id : m_handler_ids) {
    m_ui_context_ptr->removeKeyPressedHandler(id);
  }
}

void App::update(double dt) {
  m_root_renderer_ptr->update(dt);
}

void App::render(double dt) {
  m_root_renderer_ptr->render(dt);

  if (m_show_demo_window) {
    ImGui::ShowDemoWindow();
  }
}

void App::processInput(GLFWwindow* window, float dt) {
  m_root_renderer_ptr->processInput(window, dt);
}

void App::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  m_root_renderer_ptr->cursorPosCallback(window, xpos, ypos);
}

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (m_ui_context_ptr->isFocusedInGame()) {
    m_root_renderer_ptr->keyCallback(window, key, scancode, action, mods);
  }

  // if (action == GLFW_PRESS) {
    // switch (key) {
      // case GLFW_KEY_F12:
        // m_show_demo_window = !m_show_demo_window;
        // break;
    // }
  // }
}

void App::windowSizeCallback(GLFWwindow* window, int width, int height) {
  m_root_renderer_ptr->windowSizeCallback(window, width, height);
}

void App::focusCallback(bool focusedInGame) {
  m_root_renderer_ptr->focusCallback(focusedInGame);
}
