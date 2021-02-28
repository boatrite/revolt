#include "camera.h"
#include "renderer.h"
#include "shader.h"

#include <imgui.h>

#include <memory>

class CoordinateLinesRenderer : public Renderer {
 private:
  std::shared_ptr<Camera> m_camera_ptr;
  Shader m_shader {Shader("coordinate_lines.vert", "coordinate_lines.frag")};

  unsigned int m_vao {};
  unsigned int m_vbo {};
  bool m_show {true};

 public:
  CoordinateLinesRenderer(std::shared_ptr<Camera> camera_ptr) : m_camera_ptr {camera_ptr} {
    float length = 512.0f;
    float vertices[6 * 6] {
      // X
      -length,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      length,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      // Y
      0.0f,
      -length,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      length,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      // Z
      0.0f,
      0.0f,
      -length,
      0.0f,
      0.0f,
      1.0f,
      0.0f,
      0.0f,
      length,
      0.0f,
      0.0f,
      1.0f,
    };

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }

  ~CoordinateLinesRenderer() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
  }

  void render(double dt) override {
    imguiDebugControlPanel();

    if (!m_show) {
      return;
    }

    m_shader.use();

    m_shader.setMat4("model", glm::mat4(1.0f));
    m_shader.setMat4("view", m_camera_ptr->getViewMatrix());
    m_shader.setMat4("projection", m_camera_ptr->getProjectionMatrix());

    glBindVertexArray(m_vao);
    glDrawArrays(GL_LINES, 0, 6);

    glBindVertexArray(0);
  };

  void imguiDebugControlPanel() {
    if (ImGui::Begin("Info")) {
      ImGui::Separator();
      ImGui::Checkbox("Enable CoordinateLinesRenderer", &m_show);
    }
  }
};
