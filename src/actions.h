# pragma once

#include <memory>

#include "redux.hpp"

#include "ui_context.h"
#include "renderers/renderer.h"

struct AppState {
  std::shared_ptr<UIContext> ui_context_ptr;
  std::shared_ptr<Renderer> renderer_ptr {};

  AppState(std::shared_ptr<UIContext> ui_context_ptr) : ui_context_ptr{ui_context_ptr} {}
};

enum ActionType {
  SET_RENDERER
};

class Action {
  public:
    ActionType m_type;
    std::shared_ptr<Renderer> m_payload;
    Action(ActionType type, std::shared_ptr<Renderer> payload) : m_type{type}, m_payload{payload} {}

    static Action setRenderer(std::shared_ptr<Renderer> renderer_ptr) {
      return Action(SET_RENDERER, std::move(renderer_ptr));
    }
};

std::shared_ptr<AppState> appStateReducer(std::shared_ptr<AppState> state, Action action);

using AppStore = redux::Store<std::shared_ptr<AppState>, Action>;
