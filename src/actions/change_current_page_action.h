#pragma once

#include "../components/current_page.h"
#include "../rendering/renderer.h"

#include <entt/entt.hpp>

auto ChangeCurrentPageAction = [](std::shared_ptr<Renderer> renderer_ptr) {
  return [=](entt::registry& registry) {
    registry.set<CurrentPage>(renderer_ptr);
  };
};
