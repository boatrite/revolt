#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include "chunk.h"
#include "services/greedy_mesh.h"

Chunk::Chunk(glm::vec3 position, float scale) : m_position{position} {
  m_blocks.resize(pow(CHUNK_SIZE * (1.0 / scale), 3), Block{Block::Type::NONE});
  m_blocks[0] = Block{Block::Type::GRASS};
  m_blocks[1] = Block{Block::Type::DIRT};
  m_blocks[2] = Block{Block::Type::GRASS};
  m_mesh = GreedyMesh::computeChunkMesh(this);
  m_is_mesh_dirty = true;
};

Block Chunk::blockAt(int x, int y, int z) const {
  // FIXME These don't account for scale factor
  // if (x > CHUNK_SIZE - 1)
    // throw std::invalid_argument("X is larger than CHUNK_SIZE - 1");
  // if (y > CHUNK_SIZE - 1)
    // throw std::invalid_argument("Y is larger than CHUNK_SIZE - 1");
  // if (z > CHUNK_SIZE - 1)
    // throw std::invalid_argument("Z is larger than CHUNK_SIZE - 1");

  auto index { z * CHUNK_SIZE_SQUARED + y * CHUNK_SIZE + x };
  return m_blocks.at(index);
}

void Chunk::render(const Shader& shader) {
  if (!m_vao) {
    glGenVertexArrays(1, &m_vao);
  }

  if (!m_vbo) {
    glGenBuffers(1, &m_vbo);
  }

  if (m_is_mesh_dirty) {
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<float>(m_mesh.size()) * sizeof(float), m_mesh.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    m_is_mesh_dirty = false;
  }

  // Only render the chunk if the mesh is up to date. I'm not sure how
  // necessary this is.
  if (!m_is_mesh_dirty) {
    glm::mat4 block_model = glm::mat4(1.0);
    block_model = glm::translate(block_model, m_position * CHUNK_SIZE);
    shader.setMat4("model", block_model);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_mesh.size() / 6);
  }
};
