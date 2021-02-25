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
  int scale_factor {}; // 0, 1, 2, 3, ...
  std::vector<std::shared_ptr<Chunk>> chunks {};

  float inverse_scale() const { // 1, 2, 4, 8, ..., 2^n
    return pow(2.0f, scale_factor);
  }

  float inverse_scale_squared() const { // 1, 4, 16, 64, ..., 2^2n
    return pow(inverse_scale(), 2.0f);
  }

  float scale() const { // 1, 1/2, 1/4, 1/8, ..., 1/2^n
    return 1.0f / inverse_scale();
  }

  void raycast(glm::vec3 origin, glm::vec3 direction, float radius, std::function<bool(float, float, float, const Block&, glm::vec3&)> callback) const {
    glm::vec3 ray = glm::normalize(direction);
    assert(ray.length() != 0);

    // Track and display the number of times the raycast loop happens
    int cycles { 0 };

    // Cube containing the starting point.
    auto current_voxel = glm::floor(origin / scale());

    // The direction of the ray, the direction to increment x,y,z when
    // stepping.
    glm::vec3 step = glm::sign(ray);

    // Get distance to travel along ray to reach cube boundary on each axis.
    // The initial values depend on the fractional part of the origin.
    auto intbound = [=](float s, float ds) -> float {
      auto s_is_integer = round(s) == s;
      if (ds < 0 && s_is_integer) {
        return 0;
      }
      return (ds > 0 ? std::ceil(s) - s : s - std::floor(s)) / std::abs(ds);
    };
    auto t_max = glm::vec3(
      intbound(origin.x / scale(), ray.x / scale()),
      intbound(origin.y / scale(), ray.y / scale()),
      intbound(origin.z / scale(), ray.z / scale())
    );

    // The change in t when taking a step (always positive).
    auto t_delta = glm::vec3(
      scale() / ray.x * step.x,
      scale() / ray.y * step.y,
      scale() / ray.z * step.z
    );

    // Buffer for reporting faces to the callback.
    auto face = glm::vec3 {};

    // Maximum bounds of the world in block coords
    auto world_bounds = glm::vec3(width_in_chunks, height_in_chunks, length_in_chunks)
      * Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS * inverse_scale();

    static const char* window_title { "World#raycast" };
    if (ImGui::Begin(window_title)) {
      ImGui::Text("Initial Values:");
      ImGui::Text("origin        = (%f, %f, %f)", origin.x, origin.y, origin.z);
      ImGui::Text("current_voxel = (%f, %f, %f)", current_voxel.x, current_voxel.y, current_voxel.z);
      ImGui::Text("ray           = (%f, %f, %f)", ray.x, ray.y, ray.z);
      ImGui::Text("step          = (%f, %f, %f)", step.x, step.y, step.z);
      ImGui::Text("t_max         = (%f, %f, %f)", t_max.x, t_max.y, t_max.z);
      ImGui::Text("t_delta       = (%f, %f, %f)", t_delta.x, t_delta.y, t_delta.z);
      ImGui::Separator();
    }

    /* While ray has not gone past bounds of world, do the following:
     *
     * First check if we are within the world bounds. It's possible for the
     * origin (e.g. the camera) to be outside of the world viewing it.
     *
     * If we are outside, we skip any further checking and just increment and
     * check again.
     *
     * If we are inside, we find the block at that position and check if it's
     * not empty. If it's not empty, the provided callback is called.
     *
     * If the callback returns true, the loop breaks and we stop searching.
     *
     * If we search to the given radius, the loop breaks and we stop searching.
     *
     * TODO A possible improvement is making it possible for the world to be
     * aligned anywhere, not necessarily having 0,0,0 as the minimum point.
     * I believe it would be fairly trivial, replace 0s with a min_x, min_y,
     * min_z and world_bounds.xyz with world_bounds.max_xyz.
     *
     * */
    while (
      (step.x > 0 ? current_voxel.x < world_bounds.x: current_voxel.x >= 0) &&
      (step.y > 0 ? current_voxel.y < world_bounds.y: current_voxel.y >= 0) &&
      (step.z > 0 ? current_voxel.z < world_bounds.z : current_voxel.z >= 0)
    ) {
      // I guess I'm 1-indexing cycles. I feel like it makes sense that if you
      // see "1" it's the first cycle, and once the loop breaks, this will
      // contain the cycle on which we broke, so the number displayed is the
      // last cycle.
      cycles++;

      // If we are within the bounds of the world, check the block at that
      // position.
      bool in_world_bounds = !(current_voxel.x < 0 ||
                               current_voxel.y < 0 ||
                               current_voxel.z < 0 ||
                               current_voxel.x >= world_bounds.x ||
                               current_voxel.y >= world_bounds.y ||
                               current_voxel.z >= world_bounds.z);
      if (in_world_bounds) {
        auto chunk_position = Chunk::chunkPosition(current_voxel * scale());
        int chunk_index =
          chunk_position.x * height_in_chunks * length_in_chunks +
          chunk_position.y * width_in_chunks +
          chunk_position.z;
        const auto& chunk_ptr = chunks.at(chunk_index);

        auto block_position = (current_voxel - Chunk::CHUNK_SIZE_IN_UNIT_BLOCKS * chunk_position * inverse_scale());
        const auto& block = chunk_ptr->blockAt(block_position);

        // If there is a block at the current position, then we call the
        // provided callback. If that callback returns true, then we're done,
        // so we break from the loop and return.
        if (block != EMPTY_BLOCK) {
          if (callback(current_voxel.x, current_voxel.y, current_voxel.z, block, face)) {
            // Now that we're done searching, display some final information.
            if (ImGui::Begin(window_title)) {
              ImGui::Text("Final Values:");
              ImGui::Text("cycles           = %i", cycles);
              ImGui::Text("t_max            = (%f, %f, %f)", t_max.x, t_max.y, t_max.z);
              ImGui::Text("current_voxel    = (%f, %f, %f)", current_voxel.x, current_voxel.y, current_voxel.z);
              ImGui::Text("in world coords  = (%f, %f, %f)", current_voxel.x * scale(), current_voxel.y * scale(), current_voxel.z * scale());
              ImGui::Text("face             = (%f, %f, %f)", face.x, face.y, face.z);
              ImGui::Text("chunk_position   = (%f, %f, %f)", chunk_position.x, chunk_position.y, chunk_position.z);
              ImGui::Text("chunk_index      = %i", chunk_index);
              ImGui::Text("block_position   = (%f, %f, %f)", block_position.x, block_position.y, block_position.z);
              std::stringstream ss {};
              ss << block;
              ImGui::Text("block            = %s", ss.str().c_str());
            }

            break;
          }
        }
      }

      // t_max stores the t-value at which we cross a cube boundary along each
      // axis. Therefore, choosing the least t_max chooses the closest cube
      // boundary. Only the first case of the four has been commented in
      // detail.
      if (t_max.x < t_max.y) {
        if (t_max.x < t_max.z) {
          if (t_max.x > radius) break;
          // Update which cube we are now in.
          current_voxel.x += step.x;

          // Adjust t_max.x to the next X-oriented boundary crossing.
          t_max.x += t_delta.x;

          // Record the normal vector of the cube face we entered.
          face.x = -step.x;
          face.y = 0;
          face.z = 0;
        } else {
          if (t_max.z > radius) break;
          current_voxel.z += step.z;
          t_max.z += t_delta.z;

          face.x = 0;
          face.y = 0;
          face.z = -step.z;
        }
      } else {
        if (t_max.y < t_max.z) {
          if (t_max.y > radius) break;
          current_voxel.y += step.y;
          t_max.y += t_delta.y;

          face.x = 0;
          face.y = -step.y;
          face.z = 0;
        } else {
          // Identical to the second case, repeated for simplicity in
          // the conditionals.
          if (t_max.z > radius) break;
          current_voxel.z += step.z;
          t_max.z += t_delta.z;

          face.x = 0;
          face.y = 0;
          face.z = -step.z;
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
