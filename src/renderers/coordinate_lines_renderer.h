#include <memory>

#include "../camera.h"
#include "../util/shader.h"
#include "renderer.h"

class CoordinateLinesRenderer : public Renderer {
  private:
    std::shared_ptr<Camera> m_camera_ptr;
    Shader m_shader { Shader("coordinate_lines.vert", "coordinate_lines.frag") };

    unsigned int m_x_axis_vao {};
    unsigned int m_x_axis_vbo {};
    unsigned int m_y_axis_vao {};
    unsigned int m_y_axis_vbo {};
    unsigned int m_z_axis_vao {};
    unsigned int m_z_axis_vbo {};

  public:
    CoordinateLinesRenderer(std::shared_ptr<Camera> camera_ptr) : m_camera_ptr { camera_ptr } {
      // I need to get this working still, but once I do, I feel like I could
      // simplify this code a bit. Instead of 3 VAOs, I could just use one in
      // some default position, rotate it, color it, and render it. Is that
      // better?
      //
      // I also want to do some sort of chunk border renderer and probably a
      // block border renderer. What I'm actually thinking now is that I just
      // build out a giant mesh and send all the vertices precomputed and stored
      // in a single VBO. But idk if there is a gl render method for groups of
      // lines...
      //
      // Okay yeah GL_LINES interprets the vertex stream as being distinct pairs
      // of lines. So I can build up a grid or whatever.
      //
      // In which case, I could just have a single vertices array computed based
      // off of a few things like length to make the axis (right now it's 512 in
      // both directions).

      float x_axis_vertices[6] {
        -512.0f, 0.0f, 0.0f,
          512.0f, 0.0f, 0.0f,
      };

      float y_axis_vertices[6] {
        0.0f, -512.0f, 0.0f,
          0.0f, 512.0f, 0.0f
      };

      float z_axis_vertices[6] {
        0.0f, 0.0f, -512.0f,
          0.0f, 0.0f, 512.0f,
      };

      glGenVertexArrays(1, &m_x_axis_vao);
      glGenBuffers(1, &m_x_axis_vbo);

      glGenVertexArrays(1, &m_y_axis_vao);
      glGenBuffers(1, &m_y_axis_vbo);

      glGenVertexArrays(1, &m_z_axis_vao);
      glGenBuffers(1, &m_z_axis_vbo);

      glBindVertexArray(m_x_axis_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_x_axis_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(x_axis_vertices), x_axis_vertices, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindVertexArray(m_y_axis_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_y_axis_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(y_axis_vertices), y_axis_vertices, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);

      glBindVertexArray(m_z_axis_vao);
      glBindBuffer(GL_ARRAY_BUFFER, m_z_axis_vbo);
      glBufferData(GL_ARRAY_BUFFER, sizeof(z_axis_vertices), z_axis_vertices, GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
      glEnableVertexAttribArray(0);
    }

    ~CoordinateLinesRenderer() {
      glDeleteBuffers(1, &m_x_axis_vbo);
      glDeleteBuffers(1, &m_y_axis_vbo);
      glDeleteBuffers(1, &m_z_axis_vbo);

      glDeleteVertexArrays(1, &m_x_axis_vao);
      glDeleteVertexArrays(1, &m_y_axis_vao);
      glDeleteVertexArrays(1, &m_z_axis_vao);
    }

    void render(double dt) override {
      m_shader.use();

      m_shader.setMat4("model", glm::mat4(1.0f));
      m_shader.setMat4("view", m_camera_ptr->getViewMatrix());
      m_shader.setMat4("projection", m_camera_ptr->getProjectionMatrix());

      glBindVertexArray(m_x_axis_vao);
      m_shader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
      glDrawArrays(GL_LINES, 0, 2);

      glBindVertexArray(m_y_axis_vao);
      m_shader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
      glDrawArrays(GL_LINES, 0, 2);

      glBindVertexArray(m_z_axis_vao);
      m_shader.setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
      glDrawArrays(GL_LINES, 0, 2);

      glBindVertexArray(0);
    };
};
