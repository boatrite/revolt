# pragma once

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include <glm/glm.hpp>

#include "common.h"
#include "chunk.h"
#include "redux.hpp"

struct State {
  std::string world_seed {};
  int world_size {};
  std::vector<std::shared_ptr<Chunk>> chunks {};

  bool wireframe { true };
  int scale_factor { 0 };

  float scale() {
    return 1.0f / pow(2.0, scale_factor);
  }

  State(const State&) = delete; // Delete copy constructor
  State& operator=(const State&) = delete; // Delete copy assignment
  State(State&&) = delete; // Delete move constructor
  State& operator=(State &&) = delete; // Delete move assignment
};

// I've tried out lots of variations, here
// (https://devtato.com/2020/09/30/flux-architecture-part-4-message-types/) is a
// dispatch method that uses template overloading stuff to handle actions which
// is similar to something I was researching earlier.
//
// Another flux implementation: https://github.com/eandritskiy/flux_cpp
class Action {
  public:
    virtual ~Action() {
      std::cout << "Action destroyed" << std::endl;
    };
    virtual void operator()(State& state) {};
    Action(const Action&) = delete; // Delete copy constructor
    Action& operator=(const Action&) = delete; // Delete copy assignment

  protected:
    Action() {};
};

class CreateNewWorldAction : public Action {
  public:
    CreateNewWorldAction(std::string seed) : m_seed{seed} {};

    void operator()(State& state) override {
      std::cout << "In new CREATE_NEW_WORLD action handler" << std::endl;
      state.world_seed = m_seed;
      state.world_size = 2;
      for (auto i = 0; i < state.world_size; ++i) {
        for (auto ii = 0; ii < state.world_size; ++ii) {
          state.chunks.push_back(std::make_shared<Chunk>(glm::vec3(i, 0, ii), state.scale()));
        }
      }
    };

  private:
    std::string m_seed;
};

class RecreateChunksAction : public Action {
  public:
    void operator()(State& state) override {
      for (auto i = 0; i < state.world_size; ++i) {
        for (auto ii = 0; ii < state.world_size; ++ii) {
          auto index { ii * state.world_size + i };
          state.chunks[index] = std::make_shared<Chunk>(glm::vec3(i, 0, ii), state.scale());
        }
      }
    };
};

using Store = redux::Store<State, std::shared_ptr<Action>>;
