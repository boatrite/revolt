#pragma once

#include "../renderers/renderer.h"

#include <memory>

struct CurrentPage {
  std::shared_ptr<Renderer> renderer_ptr {};
};
