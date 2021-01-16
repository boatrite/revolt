# pragma once

#include <imgui.h>

class FontBook {
  public:
    static void loadFonts() {
      // By loading Inconsolata18 first, it becomes our default.
      Inconsolata18();
      Inconsolata28();
    }

    static ImFont* Inconsolata28() {
      static ImFont* s_inconsolata_28 {};
      if (!s_inconsolata_28) {
        ImGuiIO& io = ImGui::GetIO();
        s_inconsolata_28 = io.Fonts->AddFontFromFileTTF("font.ttf", 28);
      }
      return s_inconsolata_28;
    }

    static ImFont* Inconsolata18() {
      static ImFont* s_inconsolata_18 {};
      if (!s_inconsolata_18) {
        ImGuiIO& io = ImGui::GetIO();
        s_inconsolata_18 = io.Fonts->AddFontFromFileTTF("font.ttf", 18);
      }
      return s_inconsolata_18;
    }
};
