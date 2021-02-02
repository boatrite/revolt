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
  std::array<std::shared_ptr<Chunk>, 100> chunks {};

  bool wireframe { true };
  int scale_factor { 3 };

  float scale() {
    return 1.0f / pow(2.0, scale_factor);
  }

  // FIXME If I don't want to copy state all the time (which seems dangerous),
  // I need to update redux.hpp to use references where appropriate.
  // State(const State&) = delete; // Delete copy constructor
  // State& operator=(const State&) = delete; // Delete copy assignment
};

// I've tried out lots of variations, here
// (https://devtato.com/2020/09/30/flux-architecture-part-4-message-types/) is a
// dispatch method that uses template overloading stuff to handle actions which
// is similar to something I was researching earlier.
//
// Another flux implementation: https://github.com/eandritskiy/flux_cpp
class Action {
  public:
    enum class Type {
      CREATE_NEW_WORLD
    };

  public:
    virtual ~Action() {
      std::cout << "Action destroyed" << std::endl;
    };
    virtual void operator()(State& state) {};
    Action(const Action&) = delete; // Delete copy constructor
    Action& operator=(const Action&) = delete; // Delete copy assignment

    const Type getType() const {
      return m_type;
    };

  protected:
    Action(Type type) : m_type{type} {};

  private:
    const Type m_type;
};

class CreateNewWorldAction : public Action {
  public:
    CreateNewWorldAction(std::string seed) : Action { Type::CREATE_NEW_WORLD }, m_seed{seed} {};

    void operator()(State& state) override {
      std::cout << "In new CREATE_NEW_WORLD action handler" << std::endl;
      state.world_seed = m_seed;
      state.world_size = 2;
      for (auto i = 0; i < state.world_size; ++i) {
        for (auto ii = 0; ii < state.world_size; ++ii) {
          auto index { ii * CHUNK_SIZE + i };
          state.chunks[index] = std::make_shared<Chunk>(glm::vec3(i, 0, ii), state.scale());
        }
      }
    };

  private:
    std::string m_seed;
};

using Store = redux::Store<State, std::shared_ptr<Action>>;
