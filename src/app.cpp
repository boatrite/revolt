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
  m_ui_context_ptr->focusInGUI();

  ui_context_ptr->addKeyPressedHandler(
      GLFW_KEY_ESCAPE,
      this,
      [=]() { ui_context_ptr->quit(); }
  );

  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_F12,
    this,
    [=]() {
      m_show_demo_window = !m_show_demo_window;
    }
  );

  m_ui_context_ptr->addKeyPressedHandler(
    GLFW_KEY_E,
    this,
    [=]() {
      if (m_ui_context_ptr->isFocusedInGame()) {
        m_ui_context_ptr->focusInGUI();
      } else {
        m_ui_context_ptr->focusInGame();
      }
    }
  );

}

App::~App() {
  m_ui_context_ptr->removeHandlers(this);
}

void App::update(double dt) {
}

void App::render(double dt) {
  m_root_renderer_ptr->render(dt);

  if (m_show_demo_window) {
    ImGui::ShowDemoWindow();
  }
}
