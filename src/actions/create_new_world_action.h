#pragma once

#include "../components/world.h"

#include <entt/entt.hpp>

#include <string>

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
            std::make_shared<Chunk>(glm::vec3(chunk_x, chunk_y, chunk_z), world.scale()));
        }
      }
    }
  };
};
