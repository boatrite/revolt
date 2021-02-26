#pragma once

#include "../ui_context.h"

#include <imgui.h>

class MainMenuBar {
 private:
  const UIContext& m_ui_context;

 public:
  MainMenuBar(const UIContext& ui_context);
  void show();
};
