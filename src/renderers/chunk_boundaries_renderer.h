#include <memory>

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include "../camera.h"
#include "../common.h"
#include "../util/shader.h"
#include "renderer.h"

class ChunkBoundariesRenderer : public Renderer {
  private:
    std::shared_ptr<UIContext> m_ui_context_ptr;
    std::shared_ptr<Camera> m_camera_ptr;
    Shader m_shader { Shader("coordinate_lines.vert", "coordinate_lines.frag") };

    unsigned int m_vao {};
    unsigned int m_vbo {};
    bool m_show { true };

    static const int s_floats_per_vertex = 6; // x, y, z, r, g, b
    std::vector<float> m_vertices {};

  public:
    ChunkBoundariesRenderer(std::shared_ptr<UIContext> ui_context_ptr, std::shared_ptr<Camera> camera_ptr)
      : m_ui_context_ptr{ui_context_ptr}, m_camera_ptr{camera_ptr}
    {
      m_ui_context_ptr->addKeyPressedHandler(
        GLFW_KEY_F3,
        this,
        [=]() { m_show = !m_show; }
      );

      // Make 4 lines, one in each corner of the chunk.
      // These lines are going to be rendered offset by the chunk position,
      // so they should show up surrounding the chunk
      auto start = glm::vec3(0.0f, 0.0f, 0.0f);
      auto end = glm::vec3(0.0f, Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS, 0.0f);
      auto color = glm::vec3(1.0f, 1.0f, 0.0f);
      auto add_line = [=](const glm::vec3& start, const glm::vec3& end, const glm::vec3& color) {
        m_vertices.push_back(start.x);
        m_vertices.push_back(start.y);
        m_vertices.push_back(start.z);
        m_vertices.push_back(color.r);
        m_vertices.push_back(color.g);
        m_vertices.push_back(color.b);
        m_vertices.push_back(end.x);
        m_vertices.push_back(end.y);
        m_vertices.push_back(end.z);
        m_vertices.push_back(color.r);
        m_vertices.push_back(color.g);
        m_vertices.push_back(color.b);
      };
      auto x_hat = glm::vec3(1.0f, 0.0f, 0.0f);
      auto z_hat = glm::vec3(0.0f, 0.0f, 1.0f);
      add_line(start, end, color);
      add_line(start + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*x_hat, end + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*x_hat, color);
      add_line(start + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*z_hat, end + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*z_hat, color);
      add_line(start + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*x_hat + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*z_hat, end + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*x_hat + Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*z_hat, color);

      glGenVertexArrays(1, &m_vao);
      glGenBuffers(1, &m_vbo);

      glBindVertexArray(m_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
      glBufferData(GL_ARRAY_BUFFER, static_cast<float>(m_vertices.size()) * sizeof(float), m_vertices.data(), GL_STATIC_DRAW);

      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
      glEnableVertexAttribArray(1);
    }

    ~ChunkBoundariesRenderer() override {
      glDeleteBuffers(1, &m_vbo);
      glDeleteVertexArrays(1, &m_vao);
    }

    void render(double dt) override {
      imguiDebugControlPanel();

      if (!m_show) {
        return;
      }

      m_shader.use();
      m_shader.setMat4("view", m_camera_ptr->getViewMatrix());
      m_shader.setMat4("projection", m_camera_ptr->getProjectionMatrix());

      glBindVertexArray(m_vao);
      for (const auto& chunk_ptr : m_ui_context_ptr->getStore().getState().world_ptr->chunks) {
        m_shader.setMat4("model", glm::translate(
          glm::mat4(1.0f),
          chunk_ptr->getPosition() * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS
        ));
        glDrawArrays(GL_LINES, 0, m_vertices.size() / s_floats_per_vertex);
      }

      glBindVertexArray(0);
    }

    void imguiDebugControlPanel() {
      if (ImGui::Begin("Info")) {
        ImGui::Separator();
        ImGui::Checkbox("Enable ChunkBoundariesRenderer (F3)", &m_show);
      }
    }
};
