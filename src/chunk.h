#pragma once

#include <array>
#include <ostream>
#include <vector>

#include <glm/glm.hpp>

#include "common.h"
#include "util/shader.h"

class Chunk {
  private:
    glm::vec3 m_position { 0, 0, 0 };
    std::vector<Block> m_blocks {};
    const float m_scale; // 1 / 2^n where n is 0,1,2,...

    // Meshing & Rendering
    std::vector<float> m_mesh {};
    bool m_is_mesh_dirty { false };
    unsigned int m_vao {};
    unsigned int m_vbo {};

  public:
    Chunk(glm::vec3 position, float scale);

    const glm::vec3& getPosition() const { return m_position; }

    Block blockAt(int x, int y, int z) const;

    void render(const Shader& shader);

    // I don't really like this here, but I'm not sure where else to put it.
    static const glm::vec3 chunkPosition(const glm::vec3& position) {
      return glm::vec3(
        (position.x > 0 ? floor : ceil)(position.x / CHUNK_SIZE),
        floor(position.y / CHUNK_SIZE),
        (position.z > 0 ? floor : ceil)(position.z / CHUNK_SIZE)
      );
    }
};
