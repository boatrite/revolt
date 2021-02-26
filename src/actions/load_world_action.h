#pragma once

#include "../components/in_memory_snapshot.h"
#include "../components/position.h"
#include "../components/world.h"

#include <cereal/archives/json.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <entt/entt.hpp>

#include <sstream>

auto LoadWorldAction = [](entt::registry& registry) {
  std::stringstream& storage = registry.ctx<InMemorySnapshot>().storage;
  // std::cout << "****** Loading: *****" << std::endl;
  // std::cout << storage.str() << std::endl;
  std::stringstream copy;
  copy << storage.str();
  cereal::JSONInputArchive input {copy};
  entt::continuous_loader loader {registry};
  loader.entities(input).component<Position, World>(input);
};
