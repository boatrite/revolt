#pragma once

#include <array>
#include <vector>

#include <glm/glm.hpp>

constexpr int CHUNK_SIZE { 16 };

struct Block {
  enum class Type {
    NONE = 0,
    GRASS,
  };

  Type type { Type::NONE };
};

struct Chunk {
  glm::vec3 position { 0, 0, 0 };
  std::vector<Block> blocks {};

  Chunk(glm::vec3 position, float scale) : position{position} {
    blocks.resize(pow(CHUNK_SIZE * (1.0 / scale), 3), Block{Block::Type::NONE});
    blocks[0] = Block{Block::Type::GRASS};
    blocks[1] = Block{Block::Type::GRASS};
    blocks[2] = Block{Block::Type::GRASS};
  };

  static const glm::vec3 chunkPosition(const glm::vec3& position) {
    return glm::vec3(
      (position.x > 0 ? floor : ceil)(position.x / CHUNK_SIZE),
      floor(position.y / CHUNK_SIZE),
      (position.z > 0 ? floor : ceil)(position.z / CHUNK_SIZE)
    );
  }
};
