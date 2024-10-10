#ifndef SHADERS_HPP
#define SHADERS_HPP

#include <glad/glad.h>
#include <iostream>
#include <string>
#include <array>

namespace ShaderSources {
  const char* vertexShaderSource = R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
      gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
  )glsl";

  const char* fragmentShaderSources[] = {
    // Black
    R"glsl(
      #version 330 core
      out vec4 FragColor;
      void main()
      {
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
      }
    )glsl",
    // White
    R"glsl(
      #version 330 core
      out vec4 FragColor;
      void main()
      {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
      }
    )glsl",
    // Red
    R"glsl(
      #version 330 core
      out vec4 FragColor;
      void main()
      {
        FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
      }
    )glsl",
    // Green
    R"glsl(
      #version 330 core
      out vec4 FragColor;
      void main()
      {
        FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
      }
    )glsl",
    // Blue
    R"glsl(
      #version 330 core
      out vec4 FragColor;
      void main()
      {
        FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
      }
    )glsl"
  };
}

struct ShaderProgram {
  unsigned int id;
  unsigned int vertexShader;
  unsigned int fragmentShader;
};

unsigned int compileShader(GLenum type, const char* source) {
  unsigned int shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    return 0;
  }
  return shader;
}

ShaderProgram createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
  ShaderProgram program;
  program.vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
  program.fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

  if (program.vertexShader == 0 || program.fragmentShader == 0) {
    std::cerr << "Failed to compile shaders" << std::endl;
    return program;
  }

  program.id = glCreateProgram();
  glAttachShader(program.id, program.vertexShader);
  glAttachShader(program.id, program.fragmentShader);
  glLinkProgram(program.id);

  int success;
  char infoLog[512];
  glGetProgramiv(program.id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(program.id, 512, NULL, infoLog);
    std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  return program;
}

std::array<ShaderProgram, 5> createAllShaderPrograms() {
  std::array<ShaderProgram, 5> programs;
  for (int i = 0; i < 5; ++i) {
    programs[i] = createShaderProgram(ShaderSources::vertexShaderSource, ShaderSources::fragmentShaderSources[i]);
  }
  return programs;
}

#endif