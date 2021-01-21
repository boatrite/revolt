#pragma once

#include <array>

#include <glm/glm.hpp>

constexpr int CHUNK_SIZE { 16 };

constexpr int CHUNK_SIZE_CUBED { CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE };

struct Block {
  enum class Type {
    NONE = 0,
    GRASS,
  };

  Type type;
};

struct Chunk {
  glm::vec3 position { 0, 0, 0 };
  std::array<Block, CHUNK_SIZE_CUBED> blocks {};

  Chunk(glm::vec3 position) : position{position} {};

  static const glm::vec3 chunkPosition(const glm::vec3& position) {
    return glm::vec3(
      (position.x > 0 ? floor : ceil)(position.x / CHUNK_SIZE),
      floor(position.y / CHUNK_SIZE),
      (position.z > 0 ? floor : ceil)(position.z / CHUNK_SIZE)
    );
  }
};
