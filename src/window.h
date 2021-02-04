#pragma once

#include <iostream>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"

#include "app.h"
#include "font_book.h"
#include "ui_context.h"

class Window {
  public:
    static int show(std::string title, int width, int height) {
      // Print any glfw errors by setting error callback before doing anything else.
      glfwSetErrorCallback([](int error, const char* description){
        std::cerr << "Error: " << error << ", Description: " << description << std::endl;
      });

      // Initialize glfw and set basic options.
      glfwInit();
      glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

      // Get better debuggging.
      if (true) { // Eventually put behind some sort of debug flag
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
      }

      // Create window, get handle, set as current.
      GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
      if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
      }
      glfwMakeContextCurrent(window);

      // Initialize opengl.
      if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
      }

      // Initialize Dear Imgui.
      IMGUI_CHECKVERSION();
      ImGui::CreateContext();
      ImGui::StyleColorsDark();
      ImGui_ImplGlfw_InitForOpenGL(window, true);
      const char* glslVersion = "#version 130";
      ImGui_ImplOpenGL3_Init(glslVersion);
      ImGuiIO& io = ImGui::GetIO();
      io.IniFilename = NULL;
      FontBook::loadFonts();

      // Configure viewport size and update it when the framebuffer size changes.
      //
      // The FramebufferSizeCallback and WindowSizeCallback differ in that the
      // FB size callback receives the width and height in pixels, to be used
      // for pixel-based calls like glViewport. The window size callback receives
      // the new size in screen coordinates.
      // https://stackoverflow.com/a/52730404
      glViewport(0, 0, width, height);
      glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int w, int h) {
        glViewport(0, 0, w, h);
      });

      // Output any errors from OpenGL.
      GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
      if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(glDebugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
      }

      // UIContext is the input (and windowing?) system. I does a few sort of
      // related but alltogether useful things. It's more-or-less a wrapper over
      // glfw, using the window pointer to make glfw calls.
      // It acts as (sort of?) as a Subject from the Observer pattern.
      // Anything can get passed a ui_context_ptr and ask to be notified when a key
      // is pressed or whatnot. The main difference is that it doesn't have a
      // Notify() method that's called, it uses the glfw callbacks to drive out
      // notifications.
      // Has random stuff like a quit() method to do the whole
      // glfwSetWindowShouldClose thing. I might add beforeQuit callbacks if that
      // becomes desirable.
      // I have this concept of "in game" or "in gui" which toggles some imgui
      // enabled/disable settings, and is used by various places to know to stop
      // accepting input. It's kinda random, but rose pretty quickly out of a real
      // need to interact with imgui controls or move around in game.
      std::shared_ptr<UIContext> ui_context_ptr = std::make_shared<UIContext>(window);

      // App -- right now it gets its update() and render() methods called in the main
      // loop, and composes other update() and render() calls to do all the updating
      // and rendering.
      std::shared_ptr<App> app_ptr = std::make_shared<App>(ui_context_ptr);

      // Main loop
      glfwSetTime(0.0);
      double lastTime = 0.0;
      while(!glfwWindowShouldClose(window)) {
        double time = glfwGetTime();
        double dt = time - lastTime;
        lastTime = time;

        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ui_context_ptr->processInput(dt);
        app_ptr->update(dt);
        app_ptr->render(dt);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
      }

      //
      // Clean up
      //
      // Do this to cleanup OpenGL resources.
      app_ptr.reset();

      ImGui_ImplOpenGL3_Shutdown();
      ImGui_ImplGlfw_Shutdown();
      ImGui::DestroyContext();
      std::cout << "imgui context destroyed." << std::endl;

      glfwTerminate();
      std::cout << "glfw terminated." << std::endl;

      return 0;
    }

  private:
    static void GLAPIENTRY glDebugOutput(
        GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam) {
      // ignore non-significant error/warning codes
      if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

      std::cout << "---------------" << std::endl;
      std::cout << "Debug message (" << id << "): " <<  message << std::endl;

      switch (source)
      {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
      } std::cout << std::endl;

      switch (type)
      {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
      } std::cout << std::endl;

      switch (severity)
      {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
      } std::cout << std::endl;
      std::cout << std::endl;
    }
};
