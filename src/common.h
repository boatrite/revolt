#pragma once

#include <array>
#include <ostream>
#include <vector>

#include <cereal/cereal.hpp>

#include "util/print.h"

struct Block {
  enum class Type {
    NONE = 0,
    GRASS,
    DIRT,
  };

  Type type { Type::NONE };

  bool operator==(const Block& other) const {
    return this->type == other.type;
  }

  bool operator!=(const Block& other) const {
    return !(*this == other);
  }

  template<typename Archive>
  void serialize(Archive &archive) {
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

inline std::ostream& operator<<(std::ostream& o, const Block& block)
{
  o << block.type;
  return o;
}

// This contains the 4 vertices (x, y, z) for a quad.
// The vertices are given in the following order:
//
// float
//   lt_x { quad.at(0) }, lt_y { quad.at(1) },  lt_z { quad.at(2) };  // left top
//   rt_x { quad.at(3) }, rt_y { quad.at(4) },  rt_z { quad.at(5) },  // right top
//   lb_x { quad.at(6) }, lb_y { quad.at(7) },  lb_z { quad.at(8) },  // left bottom
//   rb_x { quad.at(9) }, rb_y { quad.at(10) }, rb_z { quad.at(11) }, // right bottom
typedef std::array<float, 12> quad;

enum class Side {
  NORTH,
  SOUTH,
  EAST,
  WEST,
  TOP,
  BOTTOM,
};
std::ostream& operator<<(std::ostream& o, const Side side);
