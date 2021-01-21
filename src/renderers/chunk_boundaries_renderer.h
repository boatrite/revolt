#include <memory>

#include <glm/glm.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include "../camera.h"
#include "../common.h"
#include "../util/shader.h"
#include "renderer.h"

class ChunkBoundariesRenderer : public Renderer {
  private:
    std::shared_ptr<Camera> m_camera_ptr;
    Shader m_shader { Shader("coordinate_lines.vert", "coordinate_lines.frag") };

    unsigned int m_vao {};
    unsigned int m_vbo {};
    static const int s_dimensions { 3 };
    static const int s_planes_per_dimension { 5 };
    static const int s_lines_per_plane { 7 };
    static const int s_vertices_per_line { 2 };

  public:
    ChunkBoundariesRenderer(std::shared_ptr<Camera> camera_ptr) : m_camera_ptr{camera_ptr} {
      float length = 512.0f;
      const int floats_per_vertex = 6;
      float vertices[s_dimensions * s_planes_per_dimension * s_lines_per_plane * s_vertices_per_line * floats_per_vertex] {};
      for (auto dim = 0; dim < s_dimensions; ++dim) {
        auto dim_i = dim * (s_planes_per_dimension * s_lines_per_plane * s_vertices_per_line * floats_per_vertex);

        for (auto plane = 0; plane < s_planes_per_dimension; ++plane) {
          auto plane_i = plane * (s_lines_per_plane * s_vertices_per_line * floats_per_vertex);

          for (auto line = 0; line < 2*s_lines_per_plane; line += 2) {
            auto line_i = line * floats_per_vertex;
            vertices[dim_i + plane_i + line_i + (0 + dim)%3] = -length;
            vertices[dim_i + plane_i + line_i + (1 + dim)%3] = (s_lines_per_plane/2 - line/2)*CHUNK_SIZE;
            vertices[dim_i + plane_i + line_i + (2 + dim)%3] = (s_planes_per_dimension/2 - plane)*CHUNK_SIZE;
            vertices[dim_i + plane_i + line_i + 3] = 1.0f;
            vertices[dim_i + plane_i + line_i + 4] = 1.0f;
            vertices[dim_i + plane_i + line_i + 5] = 0.0f;

            vertices[dim_i + plane_i + line_i + (0 + dim)%3 + 6] = length;
            vertices[dim_i + plane_i + line_i + (1 + dim)%3 + 6] = (s_lines_per_plane/2 - line/2)*CHUNK_SIZE;
            vertices[dim_i + plane_i + line_i + (2 + dim)%3 + 6] = (s_planes_per_dimension/2 - plane)*CHUNK_SIZE;
            vertices[dim_i + plane_i + line_i + 9] = 1.0f;
            vertices[dim_i + plane_i + line_i + 10] = 1.0f;
            vertices[dim_i + plane_i + line_i + 11] = 0.0f;
          }
        }
      }

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

    ~ChunkBoundariesRenderer() override {
      glDeleteBuffers(1, &m_vbo);
      glDeleteVertexArrays(1, &m_vao);
    }

    void render(double dt) override {
      m_shader.use();

      m_shader.setMat4("model", glm::translate(
        glm::mat4(1.0f),
        Chunk::chunkPosition(m_camera_ptr->getPosition()) * CHUNK_SIZE
      ));
      m_shader.setMat4("view", m_camera_ptr->getViewMatrix());
      m_shader.setMat4("projection", m_camera_ptr->getProjectionMatrix());

      glBindVertexArray(m_vao);
      auto total_vertex_count = s_dimensions * s_planes_per_dimension * s_lines_per_plane * s_vertices_per_line;
      glDrawArrays(GL_LINES, 0, total_vertex_count);

      glBindVertexArray(0);
    }
};
