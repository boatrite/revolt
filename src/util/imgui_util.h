# pragma once

#include <imgui.h>

#include <functional>
#include <iostream>

class ImGuiUtil {
  private:
    static ImGuiStyle s_saved_style;

  public:
    static ImGuiStyle CreateStyle(std::function<void(ImGuiStyle&)> f) {
      // Duplicate the current imgui state
      const ImGuiStyle& style = ImGui::GetStyle();
      ImGuiStyle copy = style;

      // Apply edits to style
      f(copy);

      // Technically makes a copy of the copy I think, but whatever it seems
      // fine.
      return copy;
    };

    static void PushStyle(const ImGuiStyle& next_style) {
      // Backup current style
      ImGuiStyle& style = ImGui::GetStyle();
      s_saved_style = style;

      // Set new style
      style = next_style;
    };

    static void PopStyle() {
      ImGuiStyle& style = ImGui::GetStyle();
      style = s_saved_style;
    };
};
