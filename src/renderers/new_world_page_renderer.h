# pragma once

#include <memory>

#include <imgui.h>

#include "../ui_context.h"
#include "renderer.h"

class NewWorldPageRenderer : public Renderer {
  private:
    std::shared_ptr<UIContext> m_ui_context_ptr;
    ImGuiStyle m_style;

    std::string m_world_seed { "some letters" };

  public:
    NewWorldPageRenderer(std::shared_ptr<UIContext> ui_context_ptr);
    ~NewWorldPageRenderer();
    void render(double dt) override;
};
