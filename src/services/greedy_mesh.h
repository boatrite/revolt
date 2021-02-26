#pragma once

#include "../components/block.h"
#include "../components/chunk.h"

#include <array>
#include <vector>

class GreedyMesh {
 public:
  static std::vector<float> computeChunkMesh(Chunk* chunk);

 private:
  // This contains the 4 vertices (x, y, z) for a quad.
  // The vertices are given in the following order:
  //
  // float
  //   lt_x { quad.at(0) }, lt_y { quad.at(1) },  lt_z { quad.at(2) };  // left top
  //   rt_x { quad.at(3) }, rt_y { quad.at(4) },  rt_z { quad.at(5) },  // right top
  //   lb_x { quad.at(6) }, lb_y { quad.at(7) },  lb_z { quad.at(8) },  // left bottom
  //   rb_x { quad.at(9) }, rb_y { quad.at(10) }, rb_z { quad.at(11) }, // right bottom
  typedef std::array<float, 12> quad;

  enum class Side
  {
    NORTH,
    SOUTH,
    EAST,
    WEST,
    TOP,
    BOTTOM,
  };

  static void loadQuadIntoMesh(const quad& quad,
                               const Block::Type& blockType,
                               const Side& side,
                               std::vector<float>& mesh);
  // std::ostream& operator<<(std::ostream& o, const Side side);
  // std::ostream& operator<<(std::ostream& o, const Side side) {
  // if (side == Side::NORTH) {
  // o << "North";
  // } else if (side == Side::SOUTH) {
  // o << "South";
  // } else if (side == Side::EAST) {
  // o << "East";
  // } else if (side == Side::WEST) {
  // o << "West";
  // } else if (side == Side::TOP) {
  // o << "Top";
  // } else if (side == Side::BOTTOM) {
  // o << "Bottom";
  // }
  // return o;
  // }
};
