# pragma once

#include <memory>

#include <imgui.h>

#include "renderer.h"

class MainMenuRenderer : public Renderer {
  public:
    MainMenuRenderer();
    void render(double dt) override;

  private:
    ImGuiStyle ref_saved_style;
    ImGuiStyle* ref_saved_style_ptr;
};
