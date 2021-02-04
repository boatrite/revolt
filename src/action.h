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

struct State {
  std::shared_ptr<Renderer> current_page_ptr {};

  std::string world_seed {};
  int world_size {};
  std::vector<std::shared_ptr<Chunk>> chunks {};

  int scale_factor { 0 };

  float scale() {
    return 1.0f / pow(2.0, scale_factor);
  }

  State(const State&) = delete; // Delete copy constructor
  State& operator=(const State&) = delete; // Delete copy assignment
  State(State&&) = delete; // Delete move constructor
  State& operator=(State &&) = delete; // Delete move assignment
};

auto CreateNewWorldAction =
  [](std::string seed) {
    return [=](State& state) {
      state.world_seed = seed;
      state.world_size = 2;
      for (auto i = 0; i < state.world_size; ++i) {
        for (auto ii = 0; ii < state.world_size; ++ii) {
          state.chunks.push_back(std::make_shared<Chunk>(glm::vec3(i, 0, ii), state.scale()));
        }
      }
    };
  };

auto RecreateChunksAction = [](State& state) {
  for (auto i = 0; i < state.world_size; ++i) {
    for (auto ii = 0; ii < state.world_size; ++ii) {
      auto index { ii * state.world_size + i };
      state.chunks[index] = std::make_shared<Chunk>(glm::vec3(i, 0, ii), state.scale());
    }
  }
};

auto ChangeCurrentPageAction =
  [](std::shared_ptr<Renderer> renderer_ptr) {
    return [=](State& state) {
      state.current_page_ptr = renderer_ptr;
    };
  };

using Store = redux::Store<State, std::function<void(State&)>>;
