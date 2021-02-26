#pragma once

#include <cereal/cereal.hpp>

#include <ostream>

struct Block {
  enum class Type
  {
    NONE = 0,
    GRASS,
    DIRT,
  };

  Type type {Type::NONE};

  bool operator==(const Block& other) const {
    return this->type == other.type;
  }

  bool operator!=(const Block& other) const {
    return !(*this == other);
  }

  template <typename Archive>
  void serialize(Archive& archive) {
    archive(CEREAL_NVP(type));
  };
};

static constexpr struct Block EMPTY_BLOCK { Block::Type::NONE };

inline std::ostream& operator<<(std::ostream& o, const Block::Type& block_type) {
  switch (block_type) {
    case Block::Type::NONE:
      o << "None";
      break;
    case Block::Type::GRASS:
      o << "Grass";
      break;
    case Block::Type::DIRT:
      o << "dirt";
      break;
  }
  return o;
}

inline std::ostream& operator<<(std::ostream& o, const Block& block) {
  o << block.type;
  return o;
}
