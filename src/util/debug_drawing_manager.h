#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "../camera.h"
#include "shader.h"

class DebugDrawingManager {
  private:
    std::string m_line_vertex_shader { R"(
      #version 330 core

      layout (location = 0) in vec3 position;

      uniform mat4 model;
      uniform mat4 view;
      uniform mat4 projection;

      void main()
      {
        gl_Position = projection * view * model * vec4(position, 1.0);
      }
    )" };

    std::string m_line_fragment_shader { R"(
      #version 330 core

      out vec4 FragColor;

      uniform vec3 color;

      void main()
      {
         FragColor = vec4(color, 1.0f);
      }
    )" };

    Shader m_line_shader { Shader(m_line_vertex_shader, m_line_fragment_shader) };

    std::string m_point_vertex_shader {
      R"(
        #version 330 core

        layout (location = 0) in vec3 position;

        uniform mat4 model;
        uniform mat4 view;
        uniform mat4 projection;
        uniform float point_size;

        void main()
        {
          gl_Position = projection * view * model * vec4(position, 1.0);
          gl_PointSize = 8.0f;
        }
      )"
    };
    std::string m_point_fragment_shader {
      R"(
        #version 330 core

        out vec4 FragColor;

        uniform vec3 color;

        void main()
        {
           FragColor = vec4(color, 1.0f);
        }
      )"
    };
    Shader m_point_shader { Shader(m_point_vertex_shader, m_point_fragment_shader) };

  public:
    void drawLine(
      const glm::mat4& view_matrix,
      const glm::mat4& proj_matrix,
      const glm::vec3& from,
      const glm::vec3& to,
      const glm::vec3& color,
      float line_width = 1.0f
    );

    void drawPoint(
      const glm::mat4& view_matrix,
      const glm::mat4& proj_matrix,
      const glm::vec3& point,
      const glm::vec3& color,
      float point_size = 1.0f
    );
};
