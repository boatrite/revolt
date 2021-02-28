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

/*
 * SetLeftColumnWindowCount, PushNextWindowIntoLeftColumn
 */
int ImGuiHelper::s_count {1};
int ImGuiHelper::s_index {0};

void ImGuiHelper::SetLeftColumnWindowCount(int count) {
  s_count = count;
  s_index = 0;
}

void ImGuiHelper::PushNextWindowIntoLeftColumn() {
  ImVec2 display_size = ImGui::GetIO().DisplaySize;
  const int menu_bar_height {23};
  const int container_height = display_size.y - menu_bar_height;
  const int margin {10};

  const int h {(container_height - (s_count + 1) * margin) / s_count};
  const int x {margin};
  const int y {s_index * h + (s_index + 1) * margin + menu_bar_height};

  ImGui::SetNextWindowPos(ImVec2(x, y));
  ImGui::SetNextWindowSize(ImVec2(0, h));
  s_index++;
};
