#include "app.h"

void App::update(double dt) {
  m_root_renderer_ptr->update(dt);
}

void App::render(double dt) {
  m_root_renderer_ptr->render(dt);
}

void App::processInput(GLFWwindow* window, float dt) {
  m_root_renderer_ptr->processInput(window, dt);
}

void App::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  m_root_renderer_ptr->cursorPosCallback(window, xpos, ypos);
}

void App::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  m_root_renderer_ptr->keyCallback(window, key, scancode, action, mods);
}

void App::windowSizeCallback(GLFWwindow* window, int width, int height) {
  m_root_renderer_ptr->windowSizeCallback(window, width, height);
}

void App::focusCallback(bool focusedInGame) {
  m_root_renderer_ptr->focusCallback(focusedInGame);
}
