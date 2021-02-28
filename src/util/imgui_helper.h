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

  /*
   * SetLeftColumnWindowCount, PushNextWindowIntoLeftColumn
   */
 private:
  // The total number of windows to be tiled in the y-direction.
  static int s_count;

  // s_index is which window we are about to render.
  //
  // Each frame, this is reset to zero by SetLeftColumnWindowCount and then
  // incremented each time PushNextWindowIntoLeftColumn is called.
  static int s_index;
 public:
  static void SetLeftColumnWindowCount(int count);
  static void PushNextWindowIntoLeftColumn();
};
