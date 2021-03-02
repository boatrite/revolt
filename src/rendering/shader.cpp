#include "shader.h"

#include "../util/file.h"

#include <fstream>
#include <iostream>
#include <sstream>

Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath) {
  // std::cout << "[Shader] Compiling shader from path" << std::endl;
  GLuint vertex = compileShader(File::read(vertexPath), GL_VERTEX_SHADER);
  GLuint fragment = compileShader(File::read(fragmentPath), GL_FRAGMENT_SHADER);
  if (geometryPath == nullptr) {
    compileAndLinkProgram(vertex, fragment);
  } else {
    GLuint geometry = compileShader(File::read(geometryPath), GL_GEOMETRY_SHADER);
    compileAndLinkProgram(vertex, fragment, geometry);
  }
}

Shader::Shader(std::string vertexShader, std::string fragmentShader, std::string geometryShader) {
  // std::cout << "[Shader] Compiling shader from inline code" << std::endl;
  GLuint vertex = compileShader(vertexShader, GL_VERTEX_SHADER);
  GLuint fragment = compileShader(fragmentShader, GL_FRAGMENT_SHADER);
  if (geometryShader == "") {
    compileAndLinkProgram(vertex, fragment);
  } else {
    GLuint geometry = compileShader(geometryShader, GL_GEOMETRY_SHADER);
    compileAndLinkProgram(vertex, fragment, geometry);
  }
}

void Shader::use() {
  glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUnsignedInt(const std::string& name, unsigned int value) const {
  glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const {
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, glm::vec4 value) const {
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

GLuint Shader::compileShader(std::string codeString, GLenum type) {
  const char* code = codeString.c_str();

  // 2. Compile shader code
  GLuint shader;

  int success;
  char infoLog[512];

  shader = glCreateShader(type);
  glShaderSource(shader, 1, &code, NULL);
  glCompileShader(shader);
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "ERROR: Shader compilation failed. Shader:\n" << codeString << std::endl;
  }

  return shader;
}

void Shader::compileAndLinkProgram(GLuint vertex, GLuint fragment, GLuint geometry) {
  // Create Shader Program
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  if (geometry != 0) {
    glAttachShader(ID, geometry);
  }
  glLinkProgram(ID);

  int success;
  char infoLog[512];

  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cerr << "ERROR: Program linking failed" << std::endl;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (geometry != 0) {
    glDeleteShader(geometry);
  }
}
