#pragma once

#include "../ui_context.h"
#include "renderer.h"

#include <imgui.h>

#include <memory>

class MainMenuRenderer : public Renderer {
 private:
  std::shared_ptr<UIContext> m_ui_context_ptr;
  ImGuiStyle m_style;

 public:
  MainMenuRenderer(std::shared_ptr<UIContext> ui_context_ptr);
  ~MainMenuRenderer();
  void render(double dt) override;
};
