// https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/
// https://github.com/mikolalysenko/mikolalysenko.github.com/blob/gh-pages/MinecraftMeshes/js/greedy.js
// https://gist.github.com/Vercidium/a3002bd083cce2bc854c9ff8f0118d33

#include "greedy_mesh.h"

std::vector<float> GreedyMesh::computeChunkMesh(Chunk* chunk) {
  std::vector<float> mesh {};

  // Sweep over each axis (X (d = 0), Y (d = 1) and Z (d = 2))
  for (bool backFace = true, b = false; b != backFace; backFace = false, b = !b) { // A for loop that runs twice.
    for (auto sweepAxis = 0; sweepAxis < 3; ++sweepAxis) {
      int col, row, k, l, width, height;

      int orthoAxisU = (sweepAxis + 1) % 3;
      int orthoAxisV = (sweepAxis + 2) % 3;
      float currBlockCoords[3] {};

      Block::Type mask[CHUNK_SIZE_SQUARED] {};

      // q determines the direction (X, Y or Z) that we are searching
      float q[3] {};
      q[sweepAxis] = 1;

      Side side;
      if (sweepAxis == 0)      { side = backFace ? Side::WEST   : Side::EAST;  }
      else if (sweepAxis == 1) { side = backFace ? Side::BOTTOM : Side::TOP;   }
      else if (sweepAxis == 2) { side = backFace ? Side::NORTH  : Side::SOUTH; }

      // Check each slice of the chunk one at a time
      for (currBlockCoords[sweepAxis] = -1; currBlockCoords[sweepAxis] < CHUNK_SIZE;) {
        // Compute the mask
        auto n = 0;
        for (currBlockCoords[orthoAxisV] = 0; currBlockCoords[orthoAxisV] < CHUNK_SIZE; ++currBlockCoords[orthoAxisV]) {
          for (currBlockCoords[orthoAxisU] = 0; currBlockCoords[orthoAxisU] < CHUNK_SIZE; ++currBlockCoords[orthoAxisU]) {
            // chunk.isBlockAt(x,y,z) takes local-to-chunk map positions and returns true if a block exists there

            Block blockCurrent = 0 <= currBlockCoords[sweepAxis]             ? chunk->blockAt(currBlockCoords[0],        currBlockCoords[1],        currBlockCoords[2])        : EMPTY_BLOCK;
            Block blockCompare = currBlockCoords[sweepAxis] < CHUNK_SIZE - 1 ? chunk->blockAt(currBlockCoords[0] + q[0], currBlockCoords[1] + q[1], currBlockCoords[2] + q[2]) : EMPTY_BLOCK;

            // The mask is set to true if there is a visible face between two blocks,
            //   i.e. both aren't empty and both aren't blocks
            mask[n++] = (
              blockCurrent != EMPTY_BLOCK
                && blockCompare != EMPTY_BLOCK
                && blockCurrent == blockCompare
                  ? Block::Type::NONE
                  : (backFace ? blockCompare.type : blockCurrent.type)
            );
          }
        }

        ++currBlockCoords[sweepAxis];

        n = 0;

        // Generate a mesh from the mask using lexicographic ordering,
        //   by looping over each block in this slice of the chunk
        for (row = 0; row < CHUNK_SIZE; ++row) {
          for (col = 0; col < CHUNK_SIZE;) {
            if (mask[n] != Block::Type::NONE) {
              // Compute the width of this quad and store it in width
              //   This is done by searching along the current axis until mask[n + width] is false
              for (width = 1; col + width < CHUNK_SIZE && mask[n + width] != Block::Type::NONE && mask[n + width] == mask[n]; width++) { }

              // Compute the height of this quad and store it in height
              //   This is done by checking if every block next to this row (range 0 to width) is also part of the mask.
              //   For example, if width is 5 we currently have a quad of dimensions 1 x 5. To reduce triangle count,
              //   greedy meshing will attempt to expand this quad out to CHUNK_SIZE x 5, but will stop if it reaches a hole in the mask

              auto done = false;
              for (height = 1; row + height < CHUNK_SIZE; height++) {
                // Check each block next to this quad
                for (k = 0; k < width; ++k) {
                  // If there's a hole in the mask, exit
                  if (mask[n + k + height * CHUNK_SIZE] == Block::Type::NONE) {
                    done = true;
                    break;
                  }

                  if (mask[n + k + height * CHUNK_SIZE] != mask[n]) {
                    done = true;
                    break;
                  }
                }

                if (done) {
                  break;
                }
              }

              currBlockCoords[orthoAxisU] = col;
              currBlockCoords[orthoAxisV] = row;

              // du and dv determine the size and orientation of this face
              float du[3] {};
              du[orthoAxisU] = width;

              float dv[3] {};
              dv[orthoAxisV] = height;

              // Create a quad for this face. Colour, normal or textures are not stored in this block vertex format.
              quad quad {
                currBlockCoords[0]                , currBlockCoords[1]                , currBlockCoords[2]                , // Top-left vertice position
                currBlockCoords[0] + du[0]        , currBlockCoords[1] + du[1]        , currBlockCoords[2] + du[2]        , // Top right vertice position
                currBlockCoords[0] + dv[0]        , currBlockCoords[1] + dv[1]        , currBlockCoords[2] + dv[2]        , // Bottom left vertice position
                currBlockCoords[0] + du[0] + dv[0], currBlockCoords[1] + du[1] + dv[1], currBlockCoords[2] + du[2] + dv[2]  // Bottom right vertice position
              };

              loadQuadIntoMesh(quad, mask[n], side, mesh);

              // Clear this part of the mask, so we don't add duplicate faces
              for (l = 0; l < height; ++l) {
                for (k = 0; k < width; ++k) {
                  mask[n + k + l * CHUNK_SIZE] = Block::Type::NONE;
                }
              }

              // Increment counters and continue
              col += width;
              n += width;
            } else {
              col++;
              n++;
            }
          }
        }
      }
    }
  }

  return mesh;
}

// This computes all the vertices needed to render the quad, which is composed
// of 2 triangles. It also contains the texture coordinates.
//
// The order of the vertices used to compose each triangle can be seen as
// follows:
//
// {
//   rb_x, rb_y, rb_z, rb_u, rb_v, texture layer // right bottom
//   rt_x, rt_y, rt_z, rt_u, rt_v, texture layer // right top
//   lb_x, lb_y, lb_z, lb_u, lb_v, texture layer // left bottom
//
//   lt_x, lt_y, lt_z, lt_u, lt_v, texture layer // left top
//   rt_x, rt_y, rt_z, rt_u, rt_v, texture layer // right top
//   lb_x, lb_y, lb_z, lb_u, lb_v, texture layer // left bottom
// }
void GreedyMesh::loadQuadIntoMesh(const quad& quad, const Block::Type& blockType, const Side& side, std::vector<float>& mesh) {
  float rb_x { quad.at(9) }, rb_y { quad.at(10) }, rb_z { quad.at(11) }, // right bottom
        rt_x { quad.at(3) }, rt_y { quad.at(4) },  rt_z { quad.at(5) },  // right top
        lb_x { quad.at(6) }, lb_y { quad.at(7) },  lb_z { quad.at(8) },  // left bottom
        lt_x { quad.at(0) }, lt_y { quad.at(1) },  lt_z { quad.at(2) };  // left top

  float rb_u, rb_v, // right bottom
        rt_u, rt_v, // right top
        lb_u, lb_v, // left bottom
        lt_u, lt_v; // left top

  float normal_x, normal_y, normal_z;

  if (side == Side::EAST || side == Side::WEST) {
    // If x is constant,
    // then left-to-right (u-axis) is along the z-axis
    // and top-to-bottom (v-axis) is along the y-axis.
    rb_u = rb_z;
    rt_u = rt_z;
    lb_u = lb_z;
    lt_u = lt_z;

    rb_v = rb_y;
    rt_v = rt_y;
    lb_v = lb_y;
    lt_v = lt_y;

    if (side == Side::EAST) {
      normal_x = 1.0f;
      normal_y = 0.0f;
      normal_z = 0.0f;
    } else if (side == Side::WEST) {
      normal_x = -1.0f;
      normal_y = 0.0f;
      normal_z = 0.0f;
    }
  } else if (side == Side::TOP || side == Side::BOTTOM) {
    // If y is constant,
    // then left-to-right (u-axis) is along the x-axis
    // and top-to-bottom (v-axis) is along the z-axis.
    rb_u = rb_x;
    rt_u = rt_x;
    lb_u = lb_x;
    lt_u = lt_x;

    rb_v = rb_z;
    rt_v = rt_z;
    lb_v = lb_z;
    lt_v = lt_z;

    if (side == Side::TOP) {
      normal_x = 0.0f;
      normal_y = 1.0f;
      normal_z = 0.0f;
    } else if (side == Side::BOTTOM) {
      normal_x = 0.0f;
      normal_y = -1.0f;
      normal_z = 0.0f;
    }
  } else if (side == Side::NORTH || side == Side::SOUTH) {
    // If z is constant,
    // then left-to-right (u-axis) is along the x-axis
    // and top-to-bottom (v-axis) is along the y-axis.
    rb_u = rb_x;
    rt_u = rt_x;
    lb_u = lb_x;
    lt_u = lt_x;

    rb_v = rb_y;
    rt_v = rt_y;
    lb_v = lb_y;
    lt_v = lt_y;

    if (side == Side::NORTH) {
      normal_x = 0.0f;
      normal_y = 0.0f;
      normal_z = -1.0f;
    } else if (side == Side::SOUTH) {
      normal_x = 0.0f;
      normal_y = 0.0f;
      normal_z = 1.0f;
    }
  }

  // float textureLayer { Texture::getTextureIndexFromBlockType(blockType, side) };
  float textureLayer { 0.5 };

  // right bottom
  mesh.push_back(rb_x); // position
  mesh.push_back(rb_y);
  mesh.push_back(rb_z);
  mesh.push_back(rb_u); // texture
  mesh.push_back(rb_v);
  mesh.push_back(textureLayer);
  mesh.push_back(normal_x); // normal
  mesh.push_back(normal_y);
  mesh.push_back(normal_z);

  // right top
  mesh.push_back(rt_x);
  mesh.push_back(rt_y);
  mesh.push_back(rt_z);
  mesh.push_back(rt_u);
  mesh.push_back(rt_v);
  mesh.push_back(textureLayer);
  mesh.push_back(normal_x);
  mesh.push_back(normal_y);
  mesh.push_back(normal_z);

  // left bottom
  mesh.push_back(lb_x);
  mesh.push_back(lb_y);
  mesh.push_back(lb_z);
  mesh.push_back(lb_u);
  mesh.push_back(lb_v);
  mesh.push_back(textureLayer);
  mesh.push_back(normal_x);
  mesh.push_back(normal_y);
  mesh.push_back(normal_z);

  // left top
  mesh.push_back(lt_x);
  mesh.push_back(lt_y);
  mesh.push_back(lt_z);
  mesh.push_back(lt_u);
  mesh.push_back(lt_v);
  mesh.push_back(textureLayer);
  mesh.push_back(normal_x);
  mesh.push_back(normal_y);
  mesh.push_back(normal_z);

  // right top
  mesh.push_back(rt_x);
  mesh.push_back(rt_y);
  mesh.push_back(rt_z);
  mesh.push_back(rt_u);
  mesh.push_back(rt_v);
  mesh.push_back(textureLayer);
  mesh.push_back(normal_x);
  mesh.push_back(normal_y);
  mesh.push_back(normal_z);

  // left bottom
  mesh.push_back(lb_x);
  mesh.push_back(lb_y);
  mesh.push_back(lb_z);
  mesh.push_back(lb_u);
  mesh.push_back(lb_v);
  mesh.push_back(textureLayer);
  mesh.push_back(normal_x);
  mesh.push_back(normal_y);
  mesh.push_back(normal_z);
}
