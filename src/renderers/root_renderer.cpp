#include "root_renderer.h"
#include "demo_renderer.h"
#include "overlay_renderer.h"
#include "coordinate_lines_renderer.h"

RootRenderer::RootRenderer(std::shared_ptr<AppStore> store_ptr, std::shared_ptr<UIContext> ui_context_ptr) : m_store_ptr{store_ptr}, m_ui_context_ptr{ui_context_ptr} {
  m_rendererPtrs.push_back(std::make_shared<DemoRenderer>(m_camera_ptr));
  m_rendererPtrs.push_back(std::make_shared<OverlayRenderer>(m_camera_ptr));
  m_rendererPtrs.push_back(std::make_shared<CoordinateLinesRenderer>(m_camera_ptr));
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
