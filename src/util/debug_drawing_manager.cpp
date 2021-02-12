#include <vector>

#include "debug_drawing_manager.h"

void DebugDrawingManager::drawLine(
  const glm::mat4& view_matrix,
  const glm::mat4& proj_matrix,
  const glm::vec3& from,
  const glm::vec3& to,
  const glm::vec3& color,
  float line_width,
  float duration,
  bool depth_enabled
) {
  std::cout << "in drawLine" << from << ", " << to << std::endl;
  std::vector<float> s_vertices {
    from.x,
    from.y,
    from.z,
    to.x,
    to.y,
    to.z
  };

  static unsigned int s_vao {};
  static unsigned int s_vbo {};

  if (!s_vao) {
    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
  }

  glBindVertexArray(s_vao);
  glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
  glBufferData(GL_ARRAY_BUFFER, static_cast<float>(s_vertices.size()) * sizeof(float), s_vertices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  m_shader.use();
  m_shader.setMat4("view", view_matrix);
  m_shader.setMat4("projection", proj_matrix);
  m_shader.setVec3("color", color);

  glBindVertexArray(s_vao);
  m_shader.setMat4("model", glm::mat4(1.0f));
  glDrawArrays(GL_LINES, 0, s_vertices.size() / 3);

  glBindVertexArray(0);
}
