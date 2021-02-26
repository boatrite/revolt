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

auto SaveWorldAction = [](entt::registry& registry) {
  std::stringstream storage;

  {
    cereal::JSONOutputArchive output {storage};
    entt::snapshot {registry}.entities(output).component<Position, World>(output);
  }

  // std::cout << "****** Saved: *****" << std::endl;
  // std::cout << storage.str() << std::endl;
  registry.set<InMemorySnapshot>(std::move(storage));
};
