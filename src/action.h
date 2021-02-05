# pragma once

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

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

  float scale() {
    return 1.0f / pow(2.0, scale_factor);
  }
};

struct State {
  std::shared_ptr<Renderer> current_page_ptr {};

  std::shared_ptr<World> world_ptr {};

  State(const State&) = delete; // Delete copy constructor
  State& operator=(const State&) = delete; // Delete copy assignment
  State(State&&) = delete; // Delete move constructor
  State& operator=(State &&) = delete; // Delete move assignment
};

auto CreateNewWorldAction = [](std::string seed, int scale_factor) {
  return [=](State& state) {
    auto world_ptr = std::make_shared<World>();
    world_ptr->seed = seed;
    world_ptr->scale_factor = scale_factor;
    world_ptr->width_in_chunks = 8;
    world_ptr->length_in_chunks = 3;
    for (auto chunk_x = 0; chunk_x < world_ptr->width_in_chunks; ++chunk_x) {
      for (auto chunk_z = 0; chunk_z < world_ptr->length_in_chunks; ++chunk_z) {
        world_ptr->chunks.push_back(std::make_shared<Chunk>(glm::vec3(chunk_x, 0, chunk_z), world_ptr->scale()));
      }
    }

    state.world_ptr = world_ptr;
  };
};

auto RecreateChunksAction = [](int scale_factor) {
  return [=](State& state) {
    CreateNewWorldAction(state.world_ptr->seed, scale_factor)(state);
  };
};

auto ChangeCurrentPageAction =
  [](std::shared_ptr<Renderer> renderer_ptr) {
    return [=](State& state) {
      state.current_page_ptr = renderer_ptr;
    };
  };

using Store = redux::Store<State, std::function<void(State&)>>;
