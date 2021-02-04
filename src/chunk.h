#pragma once

#include <array>
#include <ostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

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
    // Chunk size is defined as this many units regardless of scale.
    // That means when the scale is smaller, we store that many more blocks
    // inside each chunk.
    // This means that, e.g., a smaller scale causes world gen like perlin
    // noise to show up in "more resolution" over the same amount of in-world
    // space.
    static const int CHUNK_SIZE_IN_UNIT_BLOCKS;

    Chunk(glm::vec3 position, float scale);

    const glm::vec3& getPosition() const { return m_position; }
    const float getScale() const { return m_scale; }
    const int getSize() const { return CHUNK_SIZE_IN_UNIT_BLOCKS * (1.0 / m_scale); }
    const int getSizeSquared() const { return pow(getSize(), 2); }

    Block blockAt(int x, int y, int z) const;

    void render(const Shader& shader);

    // Remember, chunk position is independent of scale.
    static const glm::vec3 chunkPosition(const glm::vec3& position) {
      auto scaled = position / Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
      return glm::vec3(
        position.x > 0 ? floor(scaled.x) : ceil(scaled.x),
        floor(scaled.y),
        position.z > 0 ? floor(scaled.z) : ceil(scaled.z)
      );
    }
};
