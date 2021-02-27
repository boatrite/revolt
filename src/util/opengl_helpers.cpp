#include "opengl_helpers.h"

#include <glad/glad.h>

glm::vec2 getViewportSize() {
  int scene_viewport[4];
  glGetIntegerv(GL_VIEWPORT, scene_viewport);
  return glm::vec2(scene_viewport[2] - scene_viewport[0], scene_viewport[3] - scene_viewport[1]);
};
