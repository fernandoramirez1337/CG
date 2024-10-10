#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <array>

struct ShaderProgram {
  unsigned int id;
  unsigned int vertexShader;
  unsigned int fragmentShader;
};

namespace ShaderSources {
  extern const char* vertexShaderSource;
  extern const char* fragmentShaderSources[];
}

unsigned int compileShader(unsigned int type, const char* source);
ShaderProgram createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
std::array<ShaderProgram, 5> createAllShaderPrograms();

enum ShaderProgramID {
  BLACK,
  WHITE,
  RED,
  GREEN,
  BLUE
};

#endif // SHADERS_HPP