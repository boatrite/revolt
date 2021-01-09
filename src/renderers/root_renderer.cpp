#include "root_renderer.h"
#include "demo_renderer.h"
#include "overlay_renderer.h"

RootRenderer::RootRenderer() {
  m_rendererPtrs.push_back(std::make_shared<DemoRenderer>(m_cameraPtr));
  m_rendererPtrs.push_back(std::make_shared<OverlayRenderer>(m_cameraPtr));
}

void RootRenderer::update(double dt) {
  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->update(dt);
  }
}

void RootRenderer::render(double dt) {
  if (m_wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->render(dt);
  }
}

void RootRenderer::processInput(GLFWwindow* window, float dt) {
  m_cameraPtr->processInput(window, dt);

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->processInput(window, dt);
  }
}

void RootRenderer::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  m_cameraPtr->cursorPosCallback(window, xpos, ypos);

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->cursorPosCallback(window, xpos, ypos);
  }
}

void RootRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->keyCallback(window, key, scancode, action, mods);
  }
}

void RootRenderer::windowSizeCallback(GLFWwindow* window, int width, int height) {
  m_cameraPtr->windowSizeCallback(window, width, height);

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->windowSizeCallback(window, width, height);
  }
}

void RootRenderer::focusCallback(bool focusedInGame) {
  m_cameraPtr->focusCallback(focusedInGame);

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->focusCallback(focusedInGame);
  }
}
