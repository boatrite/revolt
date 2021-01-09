#include "root_renderer.h"

RootRenderer::RootRenderer() {
  m_renderers.push_back(std::make_shared<DemoRenderer>());
}

void RootRenderer::render(double dt) {
  if (m_wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  for(auto renderer : m_renderers) {
    renderer->render(dt);
  }
}
