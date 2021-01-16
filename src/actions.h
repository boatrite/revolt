# pragma once

#include "redux.hpp"

enum Action {
  INCREMENT,
  DECREMENT
};

using AppStore = redux::Store<int, Action>;
