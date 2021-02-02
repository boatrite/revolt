#pragma once

#include <array>
#include <vector>

#include "../chunk.h"
#include "../common.h"

class GreedyMesh {
  public:
    static std::vector<float> computeChunkMesh(Chunk* chunk);

  private:
    static void loadQuadIntoMesh(const quad& quad, const Block::Type& blockType, const Side& side, std::vector<float>& mesh);
};
