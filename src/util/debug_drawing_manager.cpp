#include "debug_drawing_manager.h"

#include <vector>

void DebugDrawingManager::drawLine(
  const glm::mat4& view_matrix,
  const glm::mat4& proj_matrix,
  const glm::vec3& from,
  const glm::vec3& to,
  const glm::vec3& color,
  float line_width
) {
  std::vector<float> vertices {
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
  glBufferData(GL_ARRAY_BUFFER, static_cast<float>(vertices.size()) * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  m_line_shader.use();
  m_line_shader.setMat4("view", view_matrix);
  m_line_shader.setMat4("projection", proj_matrix);
  m_line_shader.setVec3("color", color);

  glBindVertexArray(s_vao);
  m_line_shader.setMat4("model", glm::mat4(1.0f));
  glDrawArrays(GL_LINES, 0, vertices.size() / 3);

  glBindVertexArray(0);
}


void DebugDrawingManager::drawPoint(
  const glm::mat4& view_matrix,
  const glm::mat4& proj_matrix,
  const glm::vec3& point,
  const glm::vec3& color,
  float point_size,
  bool depth_off
) {
  glEnable(GL_PROGRAM_POINT_SIZE);
  std::vector<float> vertices { point.x, point.y, point.z };

  static unsigned int s_vao2 {};
  static unsigned int s_vbo2 {};

  if (!s_vao2) {
    glGenVertexArrays(1, &s_vao2);
    glGenBuffers(1, &s_vbo2);
  }

  glBindVertexArray(s_vao2);
  glBindBuffer(GL_ARRAY_BUFFER, s_vbo2);
  glBufferData(GL_ARRAY_BUFFER, static_cast<float>(vertices.size()) * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  m_point_shader.use();
  m_point_shader.setMat4("view", view_matrix);
  m_point_shader.setMat4("projection", proj_matrix);
  m_point_shader.setVec3("color", color);
  m_point_shader.setFloat("point_size", point_size);

  glBindVertexArray(s_vao2);
  m_point_shader.setMat4("model", glm::mat4(1.0f));
  if (depth_off) {
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
    glEnable(GL_DEPTH_TEST);
  } else {
    glDrawArrays(GL_POINTS, 0, vertices.size() / 3);
  }

  glBindVertexArray(0);
}

void DebugDrawingManager::drawCube(
  const glm::mat4& view_matrix,
  const glm::mat4& proj_matrix,
  const glm::vec3& point,
  const glm::vec3& color,
  float size,
  bool depth_off
) {
  std::vector<float> vertices {
    // First three lines from corner
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f, // from origin along x
    0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, // from origin along y
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, // from origin along z

    // Each of those lines have two coming off (x)
    1.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, // from along x then along y
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 1.0f, // from along x then along z

    // Each of those lines have two coming off (y)
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f, // from along y then along x
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 1.0f, // from along y then along z

    // Each of those lines have two coming off (z)
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, // from along z then along x
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f, // from along z then along y

    // Lastly, three final lines from where the last 6 ended
    1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f, // from along x, then y, then z
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f, // from along y, then z, then x
    1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, // from along z, then x, then y
  };

  static unsigned int s_vao2 {};
  static unsigned int s_vbo2 {};

  if (!s_vao2) {
    glGenVertexArrays(1, &s_vao2);
    glGenBuffers(1, &s_vbo2);
  }

  glBindVertexArray(s_vao2);
  glBindBuffer(GL_ARRAY_BUFFER, s_vbo2);
  glBufferData(GL_ARRAY_BUFFER, static_cast<float>(vertices.size()) * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  m_line_shader.use();
  m_line_shader.setMat4("view", view_matrix);
  m_line_shader.setMat4("projection", proj_matrix);
  m_line_shader.setVec3("color", color);

  glBindVertexArray(s_vao2);
  m_line_shader.setMat4("model", glm::translate(
    glm::scale(glm::mat4(1.0f), glm::vec3(size)),
    point
  ));
  if (depth_off) {
    glDisable(GL_DEPTH_TEST);
    glDrawArrays(GL_LINES, 0, vertices.size() / 3);
    glEnable(GL_DEPTH_TEST);
  } else {
    glDrawArrays(GL_LINES, 0, vertices.size() / 3);
  }

  glBindVertexArray(0);
}
