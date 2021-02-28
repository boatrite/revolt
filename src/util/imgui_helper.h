#pragma once

#include <imgui.h>

#include <functional>
#include <iostream>

class ImGuiHelper {
  /*
   * CreateStyle, PushStyle, PopStyle
   */
 private:
  static ImGuiStyle s_saved_style;
 public:
  static ImGuiStyle CreateStyle(std::function<void(ImGuiStyle&)> f);
  static void PushStyle(const ImGuiStyle& next_style);
  static void PopStyle();
};
