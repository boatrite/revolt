#include "actions.h"

int counterReducer(int state, CounterAction action) {
  switch (action.type) {
    case CounterActionType::INCREMENT:
      return state + action.range;
    case CounterActionType::DECREMENT:
      return state - action.range;
    default:
      return state;
  }
}
