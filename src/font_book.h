#pragma once

#include <imgui.h>

class FontBook {
 public:
  static void loadFonts() {
    // By loading Inconsolata18 first, it becomes our default.
    Inconsolata18();
    Inconsolata28();
    NotoSans18();
    NotoSans18Bold();
    NotoSans28Bold();
  }

  static ImFont* Inconsolata28() {
    static ImFont* s_font_ptr {};
    if (!s_font_ptr) {
      ImGuiIO& io = ImGui::GetIO();
      s_font_ptr = io.Fonts->AddFontFromFileTTF("Inconsolata-Regular.ttf", 28);
    }
    return s_font_ptr;
  }

  static ImFont* Inconsolata18() {
    static ImFont* s_font_ptr {};
    if (!s_font_ptr) {
      ImGuiIO& io = ImGui::GetIO();
      s_font_ptr = io.Fonts->AddFontFromFileTTF("Inconsolata-Regular.ttf", 18);
    }
    return s_font_ptr;
  }

  static ImFont* NotoSans18() {
    static ImFont* s_font_ptr {};
    if (!s_font_ptr) {
      ImGuiIO& io = ImGui::GetIO();
      s_font_ptr = io.Fonts->AddFontFromFileTTF("NotoSans-Regular.ttf", 18);
    }
    return s_font_ptr;
  }

  static ImFont* NotoSans18Bold() {
    static ImFont* s_font_ptr {};
    if (!s_font_ptr) {
      ImGuiIO& io = ImGui::GetIO();
      s_font_ptr = io.Fonts->AddFontFromFileTTF("NotoSans-Bold.ttf", 18);
    }
    return s_font_ptr;
  }

  static ImFont* NotoSans28Bold() {
    static ImFont* s_font_ptr {};
    if (!s_font_ptr) {
      ImGuiIO& io = ImGui::GetIO();
      s_font_ptr = io.Fonts->AddFontFromFileTTF("NotoSans-Bold.ttf", 28);
    }
    return s_font_ptr;
  }
};
