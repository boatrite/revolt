# pragma once

#include <memory>

#include <imgui.h>

#include "../actions.h"
#include "../ui_context.h"
#include "renderer.h"

class MainMenuRenderer : public Renderer {
  private:
    std::shared_ptr<AppStore> m_store_ptr;
    std::shared_ptr<UIContext> m_ui_context_ptr;
    ImGuiStyle ref_saved_style;
    ImGuiStyle* ref_saved_style_ptr;

  public:
    MainMenuRenderer(std::shared_ptr<AppStore> store_ptr, std::shared_ptr<UIContext> ui_context_ptr);
    void render(double dt) override;
};
