# pragma once

#include <array>
#include <iostream>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "common.h"
#include "redux.hpp"

struct State {
  std::string world_seed {};
  int world_size {};
  std::array<std::shared_ptr<Chunk>, 100> chunks {};

  bool wireframe { false };
  float scale { 1.0f };

  // FIXME If I don't want to copy state all the time (which seems dangerous),
  // I need to update redux.hpp to use references where appropriate.
  // State(const State&) = delete; // Delete copy constructor
  // State& operator=(const State&) = delete; // Delete copy assignment
};

class Action {
  public:
    enum class Type {
      CREATE_NEW_WORLD
    };

  public:
    virtual ~Action() {
      std::cout << "Action destroyed" << std::endl;
    };
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
    std::string getSeed() { return m_seed; };
  private:
    std::string m_seed;
};

static State theReducer(State state, std::shared_ptr<Action> action) {
  switch(action->getType()) {
    case Action::Type::CREATE_NEW_WORLD:
      std::cout << "In CREATE_NEW_WORLD action handler" << std::endl;
      state.world_seed = static_cast<CreateNewWorldAction&>(*action).getSeed();
      state.world_size = 2;
      for (auto i = 0; i < state.world_size; ++i) {
        for (auto ii = 0; ii < state.world_size; ++ii) {
          auto index { ii * CHUNK_SIZE + i };
          state.chunks[index] = std::make_shared<Chunk>(glm::vec3(i, 0, ii));
        }
      }
      break;
  }

  return state;
};

using Store = redux::Store<State, std::shared_ptr<Action>>;
