#include "root_renderer.h"
#include "demo_renderer.h"
#include "overlay_renderer.h"
#include "coordinate_lines_renderer.h"

RootRenderer::RootRenderer(std::shared_ptr<UIContext> ui_context_ptr) : m_ui_context_ptr{ui_context_ptr} {
  m_rendererPtrs.push_back(std::make_shared<DemoRenderer>(m_camera_ptr));
  m_rendererPtrs.push_back(std::make_shared<OverlayRenderer>(m_camera_ptr));
  m_rendererPtrs.push_back(std::make_shared<CoordinateLinesRenderer>(m_camera_ptr));
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
  m_camera_ptr->processInput(dt);

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->processInput(window, dt);
  }
}

void RootRenderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->keyCallback(window, key, scancode, action, mods);
  }
}

void RootRenderer::windowSizeCallback(GLFWwindow* window, int width, int height) {
  m_camera_ptr->windowSizeCallback(window, width, height);

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->windowSizeCallback(window, width, height);
  }
}

void RootRenderer::focusCallback(bool focusedInGame) {
  m_camera_ptr->focusCallback(focusedInGame);

  for(auto rendererPtr : m_rendererPtrs) {
    rendererPtr->focusCallback(focusedInGame);
  }
}
