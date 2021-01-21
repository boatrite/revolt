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
};
