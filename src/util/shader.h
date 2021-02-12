#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO Update the conventions to match rest of the project (this file was
// originally copied from another project before I really figured out what
// conventions I liked)
class Shader {
  public:
    // The Program ID
    unsigned int ID { glCreateProgram() };

    //
    // Two constructors because I want to be able to load shaders from files,
    // and I also want to be able to specify them inline, particularly for very
    // simple shaders.
    //
    // So the first constructor takes C style strings, the paths to the
    // shader files to load. The second constructor two std::strings, the
    // inline shader code.
    //
    // Conveniently, when you call `Shader("path.vert", "path.frag")` it picks
    // the C strings constructor. To use the other one, set the code in an
    // std::string variable first and pass the variable in. e.g.
    // ```
    // std::string vertex_shader { R"(
    //   #version 330 core
    //   ...
    // )" };
    // std::string fragment_shader { R"(
    //   #version 330 core
    //   ...
    // )" };
    // Shader(vertex_shader, fragment_shader);
    // ```
    //
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    Shader(std::string vertexShader, std::string fragmentShader, std::string geometryShader = "");

    void use();

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setUnsignedInt(const std::string &name, unsigned int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 value) const;
    void setVec3(const std::string &name, glm::vec3 value) const;
    void setVec4(const std::string &name, glm::vec4 value) const;

  private:
    GLuint compileShader(std::string codeString, GLenum type);
    void compileAndLinkProgram(GLuint vertex, GLuint fragment, GLuint geometry = 0);
};
