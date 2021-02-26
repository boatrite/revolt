#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Renderer {
 public:
  virtual ~Renderer() {
    std::cout << "Renderer (" << this << ") destroyed" << std::endl;
  }
  Renderer(const Renderer&) = delete;            // Delete copy constructor
  Renderer& operator=(const Renderer&) = delete; // Delete copy assignment

  virtual void render(double dt) {};
 protected:
  Renderer() {
    std::cout << "Renderer (" << this << ") created" << std::endl;
  }
};
