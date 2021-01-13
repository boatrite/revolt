#include <iostream>

#include "main_menu_renderer.h"

MainMenuRenderer::MainMenuRenderer() {
  // I think the way I want to do this is that:
  // I take the default style, copy it, edit it, and save that copy to be
  // restored before each time I render.

  // Duplicate the current imgui state
  const ImGuiStyle& style = ImGui::GetStyle();
  std::cout << "[Constructor] &style: " << &style << std::endl;
  std::cout << "[Constructor] &(style.WindowRounding): " << &(style.WindowRounding) << std::endl;
  ref_saved_style = style;
  ref_saved_style_ptr = &ref_saved_style;
  std::cout << "[Constructor] ref_saved_style_ptr: " << ref_saved_style_ptr << std::endl;
  std::cout << "[Constructor] &ref_saved_style: " << &ref_saved_style << std::endl;
  std::cout << "[Constructor] &(ref_saved_style.WindowRounding): " << &(ref_saved_style.WindowRounding) << std::endl;

  // Apply edits to style
  ref_saved_style.WindowRounding = 0.0f;

  std::cout << std::endl;
  std::cout << "[Constructor] style.WindowRounding: " << style.WindowRounding << std::endl;
  std::cout << "[Constructor] ref_saved_style.WindowRounding: " << ref_saved_style.WindowRounding << std::endl;
}

void MainMenuRenderer::render(double dt) {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
  ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

  const auto flags =
    ImGuiWindowFlags_NoMove |
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_NoDecoration |
    ImGuiWindowFlags_AlwaysAutoResize |
    ImGuiWindowFlags_NoSavedSettings;

  // Get handle on current style
  ImGuiStyle& style = ImGui::GetStyle();
  std::cout << std::endl;
  std::cout << "[render] &style: " << &style << std::endl;
  std::cout << "[render] &(style.WindowRounding): " << &(style.WindowRounding) << std::endl;

  ImGuiStyle backup = style;
  ImGuiStyle* ref;
  ref = &backup;
  std::cout << "[render] ref: " << ref << std::endl;

  // Enable custom styles
  // For some reason, this doesn't change the value of the style pointer,
  // but it overrites the values in style with those in ref_saved_style.
  // Seems really weird to me.
  style = *ref_saved_style_ptr;
  std::cout << std::endl;
  std::cout << "[render after enable custom] &style: " << &style << std::endl;
  std::cout << "[render after enable custom] &(style.WindowRounding): " << &(style.WindowRounding) << std::endl;
  std::cout << std::endl;
  std::cout << "[render after enable custom] style.WindowRounding: " << style.WindowRounding << std::endl;

  // Render
  static bool show { true };
  ImGui::Begin("Dear Imgui Main Menu", &show, flags);
  ImGui::ShowStyleEditor();
  ImGui::End();

  // Revert to old style
  style = *ref;
}
