#pragma once

#include <array>
#include <ostream>
#include <vector>

#include <cereal/access.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include "common.h"
#include "util/shader.h"

class Chunk {
  private:
    // Serialized variables.
    // Anything needing to be saved should be defined here and used in the
    // archive method.
    // The other members are therefore all runtime values.
    glm::vec3 m_position { 0, 0, 0 };
    const float m_scale; // 1 / 2^n where n is 0,1,2,...
    std::vector<Block> m_blocks {};

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

    //
    // Two constructors. The one is used to generate the chunk's blocks first.
    // The other is used during deserialization when we want to load the current data.
    //
    Chunk(glm::vec3 position, float scale);
    Chunk(glm::vec3 position, float scale, std::vector<Block> blocks);

    const glm::vec3& getPosition() const { return m_position; }
    const float getScale() const { return m_scale; }
    const int getSize() const { return CHUNK_SIZE_IN_UNIT_BLOCKS * (1.0 / m_scale); }
    const int getSizeSquared() const { return pow(getSize(), 2); }

    Block blockAt(int x, int y, int z) const;
    Block blockAt(const glm::vec3& block_position) const;

    void render(const Shader& shader);

    //
    // Cereal serialization
    //
    template<typename Archive>
    void serialize(Archive& archive) {
      archive(CEREAL_NVP(m_position), CEREAL_NVP(m_scale), CEREAL_NVP(m_blocks));
    };

    // Since we want to serialize a (vector of) std::shared_ptr<Chunk> and because
    // Chunk doesn't have a default constructor (and I don't want to give it one),
    // we need to define this load_and_construct method.
    // See: http://uscilab.github.io/cereal/pointers.html
    template <class Archive>
    static void load_and_construct(Archive& archive, cereal::construct<Chunk>& construct) {
      glm::vec3 position {};
      archive(position);
      float scale {};
      archive(scale);
      std::vector<Block> blocks {};
      archive(blocks);
      construct(position, scale, std::move(blocks));
    };

    // Remember, chunk position is independent of scale.
    static const glm::vec3 chunkPosition(const glm::vec3& position) {
      return chunkPosition(position.x, position.y, position.z);
    }
    static const glm::vec3 chunkPosition(const int world_x, const int world_y, const int world_z) {
      float chunk_x = world_x / Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
      float chunk_y = world_y / Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
      float chunk_z = world_z / Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
      return glm::vec3(
        chunk_x > 0 ? floor(chunk_x) : ceil(chunk_x),
        chunk_y > 0 ? floor(chunk_y) : ceil(chunk_y),
        chunk_z > 0 ? floor(chunk_z) : ceil(chunk_z)
      );
    }
};
