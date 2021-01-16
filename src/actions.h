# pragma once

#include "redux.hpp"

enum class CounterActionType {
  INCREMENT,
  DECREMENT
};

class CounterAction {
  public:
    CounterActionType type;
    int range;

    CounterAction(CounterActionType type, int range) : type(type), range(range) {}
};

int counterReducer(int state, CounterAction action);

using AppStore = redux::Store<int, CounterAction>;
