#pragma once

#include "../components/world.h"
#include "create_new_world_action.h"

#include <entt/entt.hpp>

auto RecreateChunksAction = [](int scale_factor) {
  return [=](entt::registry& registry) {
    const auto& e = registry.view<World>().front();
    const auto& world = registry.get<World>(e);
    CreateNewWorldAction(world.seed, scale_factor)(registry);
  };
};
