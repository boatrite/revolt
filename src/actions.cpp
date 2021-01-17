#include <map>
#include <functional>

#include "actions.h"

std::shared_ptr<AppState> appStateReducer(std::shared_ptr<AppState> state, Action action) {
  switch (action.m_type) {
    case SET_RENDERER:
      {
        // delete state->renderer_ptr;
        state->renderer_ptr = action.m_payload;
        return state;
      }
    default:
      return state;
  }
}
