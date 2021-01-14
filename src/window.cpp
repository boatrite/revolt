#include <iostream>

#include <imgui.h>
#include "bindings/imgui_impl_glfw.h"
#include "bindings/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "window.h"

#include "app.h"
#include "ui_context.h"

std::shared_ptr<UIContext> Window::s_ui_context_ptr { nullptr };

Window::Window(std::string title, int width, int height) : m_title{title}, m_width{width}, m_height{height} {}

int Window::show() {
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
  m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), NULL, NULL);
  if (m_window == NULL) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(m_window);

  // Initialize opengl.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // Initialize Dear Imgui.
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(m_window, true);
  const char* glslVersion = "#version 130";
  ImGui_ImplOpenGL3_Init(glslVersion);
  ImGuiIO& io = ImGui::GetIO();
  io.IniFilename = NULL;

  // Configure viewport size and update it when the framebuffer size changes.
  //
  // The FramebufferSizeCallback and WindowSizeCallback differ in that the
  // FB size callback receives the width and height in pixels, to be used
  // for pixel-based calls like glViewport. The window size callback receives
  // the new size in screen coordinates.
  // https://stackoverflow.com/a/52730404
  glViewport(0, 0, m_width, m_height);
  glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  });

  // Output any errors from OpenGL.
  GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugOutput, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
  }

  // idk Something to do with handling input.
  // std::shared_ptr<UIContext> ui_context_ptr = std::make_shared<UIContext>(m_window);
  // yeah this is probably bad if anyone tried initializing multiple window objects.
  // Maybe show should just be static too. It's not like this is a real object.
  s_ui_context_ptr = std::make_shared<UIContext>(m_window);

  // Setup App instance. Events get forwarded to it to act on and to
  // pass them on.
  std::shared_ptr<App> app_ptr = std::make_shared<App>(s_ui_context_ptr);
  glfwSetWindowUserPointer(m_window, app_ptr.get());

  // Setup event handlers
  // TODO Presumably I can inline all of these since they basically just call
  // out to the UIContext instance?
  glfwSetCursorPosCallback(m_window, cursorPosCallback);
  glfwSetKeyCallback(m_window, keyCallback);
  glfwSetScrollCallback(m_window, scrollCallback);
  glfwSetWindowSizeCallback(m_window, windowSizeCallback);

  // Starting off in the game is useful when wanting to test in the game.
  // I need to figure out how to do GUIs and imgui and setting that up in a sane
  // way.
  // Starting off in gui is probably what a real program would do because it
  // opens to the main menu, not directly to gameplay.
  // TODO Eventually the Renderer itself should do this, because it's dumb for
  // Window to care about what the first thing rendered is, the renderers should
  // just take care of themselves.
  focusInGUI(m_window);

  // Main loop
  glfwSetTime(0.0);
  double lastTime = 0.0;
  while(!glfwWindowShouldClose(m_window)) {
    double time = glfwGetTime();
    double dt = time - lastTime;
    lastTime = time;

    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (isFocusedInGame(m_window)) {
      app_ptr->processInput(m_window, dt);
    }
    app_ptr->update(dt);
    app_ptr->render(dt);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
  }

  //
  // Clean up
  //
  app_ptr.reset();

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwTerminate();
  return 0;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void GLAPIENTRY Window::glDebugOutput(GLenum source,
                            GLenum type,
                            GLuint id,
                            GLenum severity,
                            GLsizei length,
                            const GLchar *message,
                            const void *userParam)
{
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

void Window::windowSizeCallback(GLFWwindow* window, int width, int height) {
  App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
  app_ptr->windowSizeCallback(window, width, height);
}


void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  s_ui_context_ptr->keyCallback(key, scancode, action, mods);
  // std::cout
    // << "key: " << key
    // << " scancode: " << scancode
    // << " action: " << action
    // << " mods: " << mods << std::endl;
  if (action == GLFW_PRESS) {
    switch (key) {
      case GLFW_KEY_ESCAPE:
      case GLFW_KEY_Q:
        glfwSetWindowShouldClose(window, 1);
        break;
      case GLFW_KEY_E:
        if (isFocusedInGame(window)) {
          focusInGUI(window);
        } else {
          focusInGame(window);
        }
        break;
    }
  }
  App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
  app_ptr->keyCallback(window, key, scancode, action, mods);
}

void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  // std::cout << "xoffset: " << xoffset << " yoffset: " << yoffset << std::endl;
}

void Window::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  if (isFocusedInGame(window)) {
    App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    app_ptr->cursorPosCallback(window, xpos, ypos);
  }
  // std::cout << "xpos: " << xpos << " ypos: " << ypos << std::endl;
}
#pragma GCC diagnostic pop

bool Window::isFocusedInGame(GLFWwindow* window) {
  // We only want to process in-game input when we are actually "in the game".
  //
  // Now that we have a GUI we want to interact with, we need to know whether
  // we are focused in game or focused in the gui, and only do input for one or
  // the other.
  int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);
  return cursorMode == GLFW_CURSOR_DISABLED;
}

bool Window::isFocusedInGUI(GLFWwindow* window) {
  return !isFocusedInGame(window);
}

void Window::focusInGame(GLFWwindow* window) {
  if (!isFocusedInGame(window)) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // Hide cursor

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NoMouse;            // Disable Mouse
    io.ConfigFlags &= ~ImGuiConfigFlags_NavEnableKeyboard; // Disable Keyboard

    App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    app_ptr->focusCallback(true); // focus == true -> focused in game
  }
}

void Window::focusInGUI(GLFWwindow* window) {
  if (!isFocusedInGUI(window)) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // Show cursor

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;          // Enable Mouse
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard

    App* app_ptr = static_cast<App*>(glfwGetWindowUserPointer(window));
    app_ptr->focusCallback(false); // focus == false -> focused in GUI
  }
}
