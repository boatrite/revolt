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
    return (ds > 0 ? std::ceil(s) - s : s - std::floor(s)) / std::abs(ds);
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
  //                    origin + t * ray,
  // except that t is not actually stored; rather, at any given point in the
  // traversal, we keep track of the *greater* t values which we would have
  // if we took a step sufficient to cross a cube boundary along that axis
  // (i.e. change the integer part of the coordinate) in the variables
  // tMaxX, tMaxY, and tMaxZ.
  void raycast(glm::vec3 origin, glm::vec3 direction, float radius, std::function<bool(float, float, float, const Block&, glm::vec3&)> callback) const {
    // SUCCESS. The "secondary" algo works 100% correctly. If I really dig in I
    // might be able to understand why.
    // Better working version of the SO impl:
    // https://gist.github.com/dogfuntom/cc881c8fc86ad43d55d8
    static bool use_primary_algo { false }; // "primary" is the one from the gist with the "neg_ray" stuff, the 'secondary' is the SO version

    glm::vec3 ray = glm::normalize(direction);
    assert(!(ray.x == 0 && ray.y == 0 && ray.z == 0));

    glm::vec3 ray_end = origin + radius*ray;

    float _bin_size = 1;
    glm::vec3 last_voxel(std::floor(ray_end[0]/_bin_size),
                         std::floor(ray_end[1]/_bin_size),
                         std::floor(ray_end[2]/_bin_size));

    // Cube containing origin point.
    float x = floor(origin.x);
    float y = floor(origin.y);
    float z = floor(origin.z);

    static const char* window_title { "World#raycast" };
    if (ImGui::Begin(window_title)) {
      ImGui::Checkbox("Use primary algo", &use_primary_algo);
      ImGui::Separator();

      ImGui::Text("origin: %f, %f, %f", origin.x, origin.y, origin.z);
      ImGui::Separator();

      ImGui::Text("Initial (x, y, z): %f, %f, %f", x, y, z);
      ImGui::Separator();

      ImGui::Text("ray_end: %f, %f, %f", ray_end.x, ray_end.y, ray_end.z);
      ImGui::Separator();

      ImGui::Text("last_voxel: %f, %f, %f", last_voxel.x, last_voxel.y, last_voxel.z);
      ImGui::Separator();
    }

    // Direction to increment x,y,z when stepping.
    float stepX, stepY, stepZ;
    if (use_primary_algo) {
      stepX = ray.x >= 0 ? 1 : -1;
      stepY = ray.y >= 0 ? 1 : -1;
      stepZ = ray.z >= 0 ? 1 : -1;
    } else {
      stepX = glm::sign(ray.x);
      stepY = glm::sign(ray.y);
      stepZ = glm::sign(ray.z);
    }

    float tMaxX, tMaxY, tMaxZ;
    if (use_primary_algo) {
      // Distance along the ray to the next voxel border from the current position (tMaxX, tMaxY, tMaxZ).
      float next_voxel_boundary_x = (x + stepX) * _bin_size; // correct
      float next_voxel_boundary_y = (y + stepY) * _bin_size; // correct
      float next_voxel_boundary_z = (z + stepZ) * _bin_size; // correct
      // tMaxX, tMaxY, tMaxZ -- distance until next intersection with voxel-border
      // the value of t at which the ray crosses the first vertical voxel boundary
      tMaxX = (ray.x != 0) ? (next_voxel_boundary_x - origin.x) / ray.x : FLT_MAX;
      tMaxY = (ray.y != 0) ? (next_voxel_boundary_y - origin.y) / ray.y : FLT_MAX;
      tMaxZ = (ray.z != 0) ? (next_voxel_boundary_z - origin.z) / ray.z : FLT_MAX;
    } else {
      // See description above. The initial values depend on the fractional
      // part of the origin.
      tMaxX = intbound(origin.x, ray.x);
      tMaxY = intbound(origin.y, ray.y);
      tMaxZ = intbound(origin.z, ray.z);
    }

    // The change in t when taking a step (always positive).
    float tDeltaX, tDeltaY, tDeltaZ;
    if (use_primary_algo) {
      tDeltaX = (ray.x != 0) ? _bin_size / ray.x * stepX : FLT_MAX;
      tDeltaY = (ray.y != 0) ? _bin_size / ray.y * stepY : FLT_MAX;
      tDeltaZ = (ray.z != 0) ? _bin_size / ray.z * stepZ : FLT_MAX;
    } else {
      tDeltaX = stepX / ray.x;
      tDeltaY = stepY / ray.y;
      tDeltaZ = stepZ / ray.z;
    }

    // Important. I believe it solves the problem described here:
    // https://stackoverflow.com/questions/27410280/fast-voxel-traversal-algorithm-with-negative-direction
    // However, I believe it also has a bug right at the boundary between ray.<xyz> <0 and ray.<xyz> > 0.
    static bool disable_neg_ray { false };
    if (use_primary_algo) {
      if (ImGui::Begin(window_title)) {
        ImGui::Checkbox("disable_neg_ray", &disable_neg_ray);
      }

      bool neg_ray { false };
      if (!disable_neg_ray) {
        if (x != last_voxel.x && ray.x < 0) { x--; neg_ray=true; }
        if (y != last_voxel.y && ray.y < 0) { y--; neg_ray=true; }
        if (z != last_voxel.z && ray.z < 0) { z--; neg_ray=true; }
      }

      if (ImGui::Begin(window_title)) {
        ImGui::Text("neg_ray: %d", neg_ray);
        ImGui::Separator();
      }
    }

    if (ImGui::Begin(window_title)) {
      ImGui::Text("ray: %f, %f, %f", ray.x, ray.y, ray.z);
      ImGui::Separator();

      ImGui::Text("step: %f, %f, %f", stepX, stepY, stepZ);
      ImGui::Separator();

      ImGui::Text("After neg ray check (x, y, z): %f, %f, %f", x, y, z);
      ImGui::Separator();
    }

    // Buffer for reporting faces to the callback.
    auto face = glm::vec3 {};

    // Maximum bounds of the world.
    auto wx = width_in_chunks * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
    auto wy = height_in_chunks * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;
    auto wz = length_in_chunks * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS;

    if (ImGui::Begin(window_title)) {
      ImGui::Text("tMax: %f, %f, %f", tMaxX, tMaxY, tMaxZ);
      ImGui::Separator();

      ImGui::Text("tDelta: %f, %f, %f", tDeltaX, tDeltaY, tDeltaZ);
      ImGui::Separator();
    }

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

        // Okay I fixed the bug here. Now it calculates index correctly. It
        // needs to correspond correctly to how the for-loops go when
        // generating the world and filling in the chunks.
        auto chunk_position = Chunk::chunkPosition(world_position);
        int index =
          chunk_position.x * height_in_chunks * length_in_chunks +
          chunk_position.y * width_in_chunks +
          chunk_position.z;
        auto chunk_ptr = chunks.at(index);

        // FIXME I think it's totally plausible there's a bug here too. I still get weird little off by tiny amounts
        // and it could totally be within the range of maybe something here is cutting off values not quite right.
        auto block_position = (world_position - Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS * chunk_position);
        auto block = chunk_ptr->blockAt(block_position);

        if (block != EMPTY_BLOCK) {
          if (callback(x, y, z, block, face)) {
            if (ImGui::Begin(window_title)) {
              ImGui::Text("Final (x, y, z): %f, %f, %f", x, y, z);
              ImGui::Text("Starts at the origin and is incremented until\na non-empty voxel is hit and the callback returns true");
              ImGui::Separator();

              ImGui::Text("face: %f, %f, %f", face.x, face.y, face.z);
              ImGui::Separator();

              ImGui::Text("chunk_position: %f, %f, %f", chunk_position.x, chunk_position.y, chunk_position.z);
              ImGui::Text("chunk index: %i", index);
              ImGui::Text("chunk ptr: %p", (void*)chunk_ptr.get());
              ImGui::Text("block_position: %f, %f, %f", block_position.x, block_position.y, block_position.z);
              // Not sure how to display an enum
              // ImGui::Text("block: %i", block);
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
