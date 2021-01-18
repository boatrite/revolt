# pragma once

#include <iostream>

#include <imgui.h>

class ImGuiUtil {
  private:
    static ImGuiStyle s_saved_style;
    static ImGuiStyle* s_saved_style_ptr;

  public:
    static void PushStyle(ImGuiStyle* style_ptr) {
      ImGuiStyle& style = ImGui::GetStyle();
      s_saved_style = style;
      s_saved_style_ptr = &s_saved_style;

      style = *style_ptr;
    };

    static void PopStyle() {
      ImGuiStyle& style = ImGui::GetStyle();
      style = *s_saved_style_ptr;
    };
};
