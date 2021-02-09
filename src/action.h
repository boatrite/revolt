# pragma once

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

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
  int length_in_chunks {};
  int width_in_chunks {};
  int scale_factor {};
  std::vector<std::shared_ptr<Chunk>> chunks {};

  float scale() const {
    return 1.0f / pow(2.0, scale_factor);
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
