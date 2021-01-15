#include "app.h"

// Instead of doing this, can one level up I do ui_context_ptr.observe(app_instance)?
// Or inside here, just pass the this pointer in? I don't know if that actually
// gets me anything tho.
// When it's deleted, it could notify the uicontext it's being deleted. Then
// maybe uicontext could remove all handlers for that instance. That would be
// very useful.
// I would no longer need to track ids, because the instance (app here) is
// basically the id for what needs to be removed. This is all assuming I can use
// the object to find and delete the handlers, but that seems totally possible.
// I'd need to pair the handler I pass in with the this pointer... agani, how do
// I compare? Can I just straight up use the pointer? I mean it's a basically a
// number right?
App::App(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_F12,
    this,
    [=]() {
      m_show_demo_window = !m_show_demo_window;
    }
  );

  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_F11,
    this,
    []() {
      std::cout << "hello from f11 key pressed callback" << std::endl;
    }
  );
}

App::~App() {
  m_ui_context_ptr->removeHandlers(this);
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

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (m_ui_context_ptr->isFocusedInGame()) {
    m_root_renderer_ptr->keyCallback(window, key, scancode, action, mods);
  }
}

void App::windowSizeCallback(GLFWwindow* window, int width, int height) {
  m_root_renderer_ptr->windowSizeCallback(window, width, height);
}

void App::focusCallback(bool focusedInGame) {
  m_root_renderer_ptr->focusCallback(focusedInGame);
}
