# pragma once

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include <entt/entt.hpp>

#include <glm/glm.hpp>

#include "chunk.h"
#include "common.h"
#include "redux.hpp"
#include "renderers/renderer.h"

struct World {
  std::string name {};
  std::string seed {};
  int length_in_chunks {};
  int width_in_chunks {};
  int scale_factor {};
  std::vector<std::shared_ptr<Chunk>> chunks {};

  float scale() const {
    return 1.0f / pow(2.0, scale_factor);
  }
};

struct CurrentPage {
  std::shared_ptr<Renderer> renderer_ptr {};
};

auto CreateNewWorldAction = [](std::string seed, int scale_factor) {
  return [=](entt::registry& registry) {
    auto& world = registry.set<World>();
    world.seed = seed;
    world.scale_factor = scale_factor;
    world.width_in_chunks = 8;
    world.length_in_chunks = 3;
    for (auto chunk_x = 0; chunk_x < world.width_in_chunks; ++chunk_x) {
      for (auto chunk_z = 0; chunk_z < world.length_in_chunks; ++chunk_z) {
        world.chunks.push_back(std::make_shared<Chunk>(glm::vec3(chunk_x, 0, chunk_z), world.scale()));
      }
    }
  };
};

auto RecreateChunksAction = [](int scale_factor) {
  return [=](entt::registry& registry) {
    const auto& world = registry.ctx<World>();
    CreateNewWorldAction(world.seed, scale_factor)(registry);
  };
};

auto ChangeCurrentPageAction =
  [](std::shared_ptr<Renderer> renderer_ptr) {
    return [=](entt::registry& registry) {
      registry.set<CurrentPage>(renderer_ptr);
    };
  };

using Store = redux::Store<entt::registry, std::function<void(entt::registry&)>>;
