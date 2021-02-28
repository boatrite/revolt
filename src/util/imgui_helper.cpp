#include "imgui_helper.h"

/*
 * CreateStyle, PushStyle, PopStyle
 */
ImGuiStyle ImGuiHelper::s_saved_style {};

ImGuiStyle ImGuiHelper::CreateStyle(std::function<void(ImGuiStyle&)> f) {
  // Duplicate the current imgui state
  const ImGuiStyle& style = ImGui::GetStyle();
  ImGuiStyle copy = style;

  // Apply edits to style
  f(copy);

  // Technically makes a copy of the copy I think, but whatever it seems
  // fine.
  return copy;
};

void ImGuiHelper::PushStyle(const ImGuiStyle& next_style) {
  // Backup current style
  ImGuiStyle& style = ImGui::GetStyle();
  s_saved_style = style;

  // Set new style
  style = next_style;
};

void ImGuiHelper::PopStyle() {
  ImGuiStyle& style = ImGui::GetStyle();
  style = s_saved_style;
};
