# pragma once

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include <imgui.h>

#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

#include <entt/entt.hpp>

#include <glm/glm.hpp>

#include "util/glm_cereal.h"

#include "chunk.h"
#include "common.h"
#include "redux.hpp"
#include "renderers/renderer.h"

struct Position {
  float x {}, y {};

  template<typename Archive>
  void serialize(Archive &archive) {
    archive(x, y);
  }
};

struct World {
  std::string name {};
  std::string seed {};
  int width_in_chunks {}; // x axis
  int height_in_chunks {}; // y axis
  int length_in_chunks {}; // z axis
  int scale_factor {};
  std::vector<std::shared_ptr<Chunk>> chunks {};

  float scale() const {
    return 1.0f / pow(2.0, scale_factor);
  }

  float intbound(float s, float ds) const {
    auto s_is_integer = round(s) == s;
    if (ds < 0 && s_is_integer) {
      return 0;
    }

    // Find the smallest positive t such that s+t*ds is an integer.
    if (ds < 0) {
      return intbound(-s, -ds);
    } else {
      s = mod(s, 1);
      // problem is now s+t*ds = 1
      return (1-s)/ds;
    }
  }

  int mod(int value, int modulus) const {
    return (value % modulus + modulus) % modulus;
  }

  void debugWindow(float x, float y, float z, float dx, float dy, float dz, float stepX, float stepY, float stepZ, float tMaxX, float tMaxY, float tMaxZ, float tDeltaX, float tDeltaY, float tDeltaZ, const glm::vec3& face, float wx, float wy, float wz) const {
    if (ImGui::Begin("World Raycast")) {
      ImGui::Text("x: %f", x);
      ImGui::Text("y: %f", y);
      ImGui::Text("z: %f", z);
      ImGui::Text("dx: %f", dx);
      ImGui::Text("dy: %f", dy);
      ImGui::Text("dz: %f", dz);
      ImGui::Text("stepZ: %f", stepX);
      ImGui::Text("stepY: %f", stepY);
      ImGui::Text("stepZ: %f", stepZ);
      ImGui::Text("tMaxX: %f", tMaxX);
      ImGui::Text("tMaxY: %f", tMaxY);
      ImGui::Text("tMaxZ: %f", tMaxZ);
      ImGui::Text("tDeltaX: %f", tDeltaX);
      ImGui::Text("tDeltaY: %f", tDeltaY);
      ImGui::Text("tDeltaZ: %f", tDeltaZ);
    }
  }

  // Source: https://gamedev.stackexchange.com/a/49423
  // Also here's a CUDA C impl: https://stackoverflow.com/a/38552664
  // ---
  // From "A Fast Voxel Traversal Algorithm for Ray Tracing"
  // by John Amanatides and Andrew Woo, 1987
  // <http://www.cse.yorku.ca/~amana/research/grid.pdf>
  // <http://citeseer.ist.psu.edu/viewdoc/summary?doi=10.1.1.42.3443>
  // Extensions to the described algorithm:
  //   • Imposed a distance limit.
  //   • The face passed through to reach the current cube is provided to
  //     the callback.
  // The foundation of this algorithm is a parameterized representation of
  // the provided ray,
  //                    origin + t * direction,
  // except that t is not actually stored; rather, at any given point in the
  // traversal, we keep track of the *greater* t values which we would have
  // if we took a step sufficient to cross a cube boundary along that axis
  // (i.e. change the integer part of the coordinate) in the variables
  // tMaxX, tMaxY, and tMaxZ.
  void raycast(glm::vec3 origin, glm::vec3 direction, float radius, std::function<bool(float, float, float, const Block&, glm::vec3&)> callback) const {
    glm::vec3 ray_end = origin + radius*glm::normalize(direction);
    glm::vec3 ray = glm::normalize(direction);

    float _bin_size = 1;
    glm::vec3 last_voxel(std::floor(ray_end[0]/_bin_size),
                         std::floor(ray_end[1]/_bin_size),
                         std::floor(ray_end[2]/_bin_size));

    // Cube containing origin point.
    float x = floor(origin.x);
    float y = floor(origin.y);
    float z = floor(origin.z);
    // Break out direction vector.
    float dx = direction.x;
    float dy = direction.y;
    float dz = direction.z;
    // Direction to increment x,y,z when stepping.
    float stepX = glm::sign(dx);
    float stepY = glm::sign(dy);
    float stepZ = glm::sign(dz);
    // See description above. The initial values depend on the fractional
    // part of the origin.
    // float tMaxX = intbound(origin.x, dx);
    // float tMaxY = intbound(origin.y, dy);
    // float tMaxZ = intbound(origin.z, dz);

    // Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).
    float next_voxel_boundary_x = (x+stepX)*_bin_size; // correct
    float next_voxel_boundary_y = (y+stepY)*_bin_size; // correct
    float next_voxel_boundary_z = (z+stepZ)*_bin_size; // correct
    // tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
    // the value of t at which the ray crosses the first vertical voxel boundary
    float tMaxX = (direction.x != 0) ? (next_voxel_boundary_x - origin.x)/direction.x : DBL_MAX;
    float tMaxY = (direction.y != 0) ? (next_voxel_boundary_y - origin.y)/direction.y : DBL_MAX;
    float tMaxZ = (direction.z != 0) ? (next_voxel_boundary_z - origin.z)/direction.z : DBL_MAX;

    // The change in t when taking a step (always positive).
    // float tDeltaX = stepX/dx;
    // float tDeltaY = stepY/dy;
    // float tDeltaZ = stepZ/dz;
    float tDeltaX = (direction.x!=0) ? _bin_size/direction.x*stepX : DBL_MAX;
    float tDeltaY = (direction.y!=0) ? _bin_size/direction.y*stepY : DBL_MAX;
    float tDeltaZ = (direction.z!=0) ? _bin_size/direction.z*stepZ : DBL_MAX;

    glm::vec3 diff(0,0,0);
    bool neg_ray = false;
    if (x != last_voxel[0] && ray[0]<0) { diff[0]--; neg_ray=true; }
    if (y != last_voxel[1] && ray[1]<0) { diff[1]--; neg_ray=true; }
    if (z != last_voxel[2] && ray[2]<0) { diff[2]--; neg_ray=true; }
    if (neg_ray) {
      x+=diff.x;
      y+=diff.y;
      z+=diff.z;
    }

    // Buffer for reporting faces to the callback.
    auto face = glm::vec3 {};

    auto wx = width_in_chunks * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
    auto wy = height_in_chunks * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
    auto wz = length_in_chunks * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;

    debugWindow(x, y, z, dx, dy, dz, stepX, stepY, stepZ, tMaxX, tMaxY, tMaxZ, tDeltaX, tDeltaY, tDeltaZ, face, wx, wy, wz);

    // Avoids an infinite loop.
    if (dx == 0 && dy == 0 && dz == 0) {
      throw "Raycast in zero direction!";
    }

    // Rescale from units of 1 cube-edge to units of 'direction' so we can
    // compare with 't'.
    // radius /= sqrt(dx*dx+dy*dy+dz*dz);

    /* While ray has not gone past bounds of world */
    while (
      (stepX > 0 ? x < wx: x >= 0) &&
      (stepY > 0 ? y < wy: y >= 0) &&
      (stepZ > 0 ? z < wz : z >= 0)
    ) {
      // Invoke the callback, unless we are not *yet* within the bounds of the
      // world.
      if (!(x < 0 || y < 0 || z < 0 || x >= wx || y >= wy || z >= wz)) {
        auto world_position = glm::vec3(x, y, z);

        auto chunk_position = Chunk::chunkPosition(world_position);
        int index =
          chunk_position.z * height_in_chunks * width_in_chunks +
          chunk_position.y * width_in_chunks +
          chunk_position.x;
        auto chunk_ptr = chunks.at(index);

        // std::cout << "***************** 1: " << (world_position - Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*chunk_position) << std::endl;
        // std::cout << "***************** 2: " << chunk_ptr->getSize() << std::endl;
        // std::cout << "***************** 3: " << (world_position - Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*chunk_position) / chunk_ptr->getSize() << std::endl;
        // auto block_position = floor(
          // (world_position - Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS*chunk_position) /
          // chunk_ptr->getSize()
        // );
        auto block_position = (world_position - Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS * chunk_position);
        auto block = chunk_ptr->blockAt(block_position);
        if (block != EMPTY_BLOCK) {
          // std::cout << "In raycast: " << x << ", " << y << ", " << z << std::endl;
          if (callback(x, y, z, block, face)) {
            if (ImGui::Begin("World Raycast")) {
              ImGui::Text("neg_ray: %d", neg_ray);
              ImGui::Text("world_position.x: %f", world_position.x);
              ImGui::Text("world_position.y: %f", world_position.y);
              ImGui::Text("world_position.z: %f", world_position.z);
              ImGui::Text("chunk_position.x: %f", chunk_position.x);
              ImGui::Text("chunk_position.y: %f", chunk_position.y);
              ImGui::Text("chunk_position.z: %f", chunk_position.z);
              ImGui::Text("chunk index: %i", index);
              ImGui::Text("chunk ptr: %p", (void*)chunk_ptr.get());
              ImGui::Text("block_position.x: %f", block_position.x);
              ImGui::Text("block_position.y: %f", block_position.y);
              ImGui::Text("block_position.z: %f", block_position.z);
              ImGui::Text("block: %i", block);
            }
            break;
          }
        }
      }

      // tMaxX stores the t-value at which we cross a cube boundary along the
      // X axis, and similarly for Y and Z. Therefore, choosing the least tMax
      // chooses the closest cube boundary. Only the first case of the four
      // has been commented in detail.
      if (tMaxX < tMaxY) {
        if (tMaxX < tMaxZ) {
          // if (tMaxX > radius) break;
          // Update which cube we are now in.
          x += stepX;
          // Adjust tMaxX to the next X-oriented boundary crossing.
          tMaxX += tDeltaX;
          // Record the normal vector of the cube face we entered.
          face[0] = -stepX;
          face[1] = 0;
          face[2] = 0;
        } else {
          // if (tMaxZ > radius) break;
          z += stepZ;
          tMaxZ += tDeltaZ;
          face[0] = 0;
          face[1] = 0;
          face[2] = -stepZ;
        }
      } else {
        if (tMaxY < tMaxZ) {
          // if (tMaxY > radius) break;
          y += stepY;
          tMaxY += tDeltaY;
          face[0] = 0;
          face[1] = -stepY;
          face[2] = 0;
        } else {
          // Identical to the second case, repeated for simplicity in
          // the conditionals.
          // if (tMaxZ > radius) break;
          z += stepZ;
          tMaxZ += tDeltaZ;
          face[0] = 0;
          face[1] = 0;
          face[2] = -stepZ;
        }
      }
      // std::cout << "In raycast after tmax: " << x << ", " << y << ", " << z << std::endl;
    }
  }


  template<typename Archive>
  void serialize(Archive &archive) {
    archive(
      CEREAL_NVP(name),
      CEREAL_NVP(seed),
      CEREAL_NVP(length_in_chunks),
      CEREAL_NVP(width_in_chunks),
      CEREAL_NVP(scale_factor),
      CEREAL_NVP(chunks)
    );
  }
};

struct CurrentPage {
  std::shared_ptr<Renderer> renderer_ptr {};
};

struct InMemorySnapshot {
  std::stringstream storage {};
};

auto CreateNewWorldAction = [](std::string seed, int scale_factor) {
  return [=](entt::registry& registry) {
    auto e0 = registry.create();
    auto& world = registry.emplace<World>(e0, "My name", seed);
    // auto& world = registry.set<World>("My name", seed);
    world.scale_factor = scale_factor;
    world.width_in_chunks = 8;
    world.height_in_chunks = 1;
    world.length_in_chunks = 3;
    for (auto chunk_x = 0; chunk_x < world.width_in_chunks; ++chunk_x) {
      for (auto chunk_y = 0; chunk_y < world.height_in_chunks; ++chunk_y) {
        for (auto chunk_z = 0; chunk_z < world.length_in_chunks; ++chunk_z) {
          world.chunks.push_back(
            std::make_shared<Chunk>(
              glm::vec3(chunk_x, chunk_y, chunk_z),
              world.scale()
            )
          );
        }
      }
    }
  };
};

auto RecreateChunksAction = [](int scale_factor) {
  return [=](entt::registry& registry) {
    const auto& e = registry.view<World>().front();
    const auto& world = registry.get<World>(e);
    CreateNewWorldAction(world.seed, scale_factor)(registry);
  };
};

auto SaveWorldAction = [](entt::registry& registry) {
  std::stringstream storage;

  {
    cereal::JSONOutputArchive output{storage};
    entt::snapshot{registry}.entities(output).component<Position, World>(output);
  }

  // std::cout << "****** Saved: *****" << std::endl;
  // std::cout << storage.str() << std::endl;
  registry.set<InMemorySnapshot>(std::move(storage));
};

// There's a funny "bug"/unintended behavior where when you call the LoadWorldAction
// and load in the new state, this causes the scale factor slider widget with callback
// to detect the value changed, which fires the RecreateChunksAction which
// recreates the world including the chunks.
//
// This is all kinds of weird. It would be good if the widget only fired when the user
// changes the value, not when it's auto changed. So fix that.
//
// And then what I really need is to be able to serialize and deserialize chunks.
// So to do that, I need to consider how to implement w/r/t the entt architecture.
// What components, what entities?
//
// I could even add in run length encoding to see compression is possible.
auto LoadWorldAction = [](entt::registry& registry) {
  std::stringstream& storage = registry.ctx<InMemorySnapshot>().storage;
  // std::cout << "****** Loading: *****" << std::endl;
  // std::cout << storage.str() << std::endl;
  std::stringstream copy;
  copy << storage.str();
  cereal::JSONInputArchive input{copy};
  entt::continuous_loader loader{registry};
  loader.entities(input).component<Position, World>(input);
};

auto ChangeCurrentPageAction =
  [](std::shared_ptr<Renderer> renderer_ptr) {
    return [=](entt::registry& registry) {
      registry.set<CurrentPage>(renderer_ptr);
    };
  };

using Store = redux::Store<entt::registry, std::function<void(entt::registry&)>>;
