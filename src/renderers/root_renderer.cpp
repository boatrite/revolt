#include "root_renderer.h"

RootRenderer::RootRenderer() {
};

void RootRenderer::render(double dt) {
  glClearColor(0.132f, 0.132f, 0.132f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
