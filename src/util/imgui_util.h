# pragma once

#include <iostream>
#include <functional>

#include <imgui.h>

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

    // This creates and returns a function which when called renders an ImGui
    // component that fires the given callback when a change to the value is
    // detected.
    static std::function<void()> SliderInt(
      const char* label,
      int* v,
      int v_min,
      int v_max,
      std::function<void()> f,
      const char* format = "%d",
      ImGuiSliderFlags flags = 0
    ) {
      return [=]() {
        static int s_last_value { *v };
        ImGui::SliderInt(label, v, v_min, v_max, format, flags);
        if (*v != s_last_value) {
          if (f) {
            f();
          }
          s_last_value = *v;
        }
      };
    }
};
