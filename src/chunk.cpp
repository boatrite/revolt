#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/scalar_multiplication.hpp>

#include <noise/noise.h>

#include "chunk.h"
#include "services/greedy_mesh.h"

const int Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS { 16 };

Chunk::Chunk(glm::vec3 position, float scale) : m_position{position}, m_scale{scale} {
  m_blocks.resize(pow(getSize(), 3), Block{Block::Type::NONE});

  noise::module::Perlin perlin;
  float min_height = 0;
  float max_height = getSize();
  for (float block_x = 0; block_x < getSize(); ++block_x) {
    for (float block_z = 0; block_z < getSize(); ++block_z) {
      float noise_x = block_x / getSize() + position.x;
      float noise_z = block_z / getSize() + position.z;

      float generated_noise = (1 + perlin.GetValue(noise_x, 0.0, noise_z)) / 2.0;
      float noise = glm::clamp(generated_noise, 0.0f, 1.0f);
      float raw_height = min_height + noise * (max_height + 1 - min_height);
      auto height = floor(glm::clamp(raw_height, min_height, max_height));

      for (auto block_y = 0; block_y < height; ++block_y) {
        auto index = block_z * getSizeSquared() + block_y * getSize() + block_x;
        m_blocks[index] = Block{Block::Type::GRASS};
      }
    }
  }

  m_mesh = GreedyMesh::computeChunkMesh(this);
  m_is_mesh_dirty = true;
};

Chunk::Chunk(glm::vec3 position, float scale, std::vector<Block> blocks) : m_position{position}, m_scale{scale}, m_blocks{std::move(blocks)} {
  m_mesh = GreedyMesh::computeChunkMesh(this);
  m_is_mesh_dirty = true;
};

Block Chunk::blockAt(int block_x, int block_y, int block_z) const {
  assert(block_x >= 0 && block_x < getSize());
  assert(block_y >= 0 && block_y < getSize());
  assert(block_z >= 0 && block_z < getSize());
  auto index { block_z * getSizeSquared() + block_y * getSize() + block_x };
  return m_blocks.at(index);
}

Block Chunk::blockAt(const glm::vec3& block_position) const {
  return blockAt(block_position.x, block_position.y, block_position.z);
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

  // Only render the chunk if the mesh is up to date.
  // I'm not sure how necessary this is.
  if (!m_is_mesh_dirty) {
    glm::mat4 block_model = glm::scale(glm::mat4(1.0), glm::vec3(m_scale));
    block_model = glm::translate(block_model, m_position * getSize());
    shader.setMat4("model", block_model);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_mesh.size() / 6);
  }
};
