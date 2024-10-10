#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "mylib.hpp"
#include "shaders.hpp"
#include "myshapes.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 732;
unsigned int renderMode = GL_LINES;

std::array<ShaderProgram, 5> shaderPrograms;

int main() {
  //create_testShape();
  create_pizza1();
  // GLFW initialization and configuration
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  // Window creation
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  // GLAD initialization
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  shaderPrograms = createAllShaderPrograms();

  const size_t verticesPerPoint = 3;
  const size_t numberOfVertices = pizza1.vertices.size() * verticesPerPoint;

  float vertices[numberOfVertices];
  pizza1.get_vertices(vertices);

  const size_t numberOfIndices = pizza1.indices[0].size();
  unsigned int indices[numberOfIndices];
  pizza1.get_indices(indices);

  const size_t numberOfIndices2 = pizza1.indices[1].size();
  unsigned int indices2[numberOfIndices2];
  pizza1.get_indices(indices2, 1);

  // Print vertices
  std::cout << "Vertices:" << std::endl;
  for (size_t i = 0; i < numberOfVertices; i += 3) {
    std::cout << "(" << vertices[i] << ", " << vertices[i+1] << ", " << vertices[i+2] << ")" << std::endl;
  }

  // Print indices
  std::cout << "\nIndices:" << std::endl;
  for (size_t i = 0; i < numberOfIndices; ++i) {
    std::cout << indices[i] << " ";
    if ((i + 1) % 3 == 0) std::cout << std::endl;  // New line every 3 indices
  }

  std::cout << "\nIndices2:" << std::endl;
  for (size_t i = 0; i < numberOfIndices2; ++i) {
    std::cout << indices2[i] << " ";
    if ((i + 1) % 3 == 0) std::cout << std::endl;  // New line every 3 indices
  }

  glGenVertexArrays(1, &pizza1.VAOs[0]);
  glGenBuffers(1, &pizza1.VBOs[0]);
  glGenBuffers(2, &pizza1.EBOs[0]);

  glBindVertexArray(pizza1.VAOs[0]);
  glBindBuffer(GL_ARRAY_BUFFER, pizza1.VBOs[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pizza1.EBOs[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pizza1.EBOs[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

  const size_t componentsPerVertex = 3;
  glVertexAttribPointer(0, componentsPerVertex, GL_FLOAT, GL_FALSE, componentsPerVertex * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Render loop
  while (!glfwWindowShouldClose(window)) {
    pizza1.get_vertices(vertices);

    glBindBuffer(GL_ARRAY_BUFFER, pizza1.VBOs[0]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numberOfVertices * sizeof(float), vertices);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (renderMode == GL_LINES) {
      glUseProgram(shaderPrograms[1].id); // White
      glBindVertexArray(pizza1.VAOs[0]);
      glDrawElements(GL_LINE_LOOP, numberOfIndices, GL_UNSIGNED_INT, 0);
    } else if (renderMode == GL_TRIANGLES) {
      glUseProgram(shaderPrograms[0].id); // Black
      glBindVertexArray(pizza1.VAOs[0]);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pizza1.EBOs[1]);
      glDrawElements(GL_TRIANGLES, numberOfIndices2, GL_UNSIGNED_INT, 0);
    } else if (renderMode == GL_POINTS) {
      glUseProgram(shaderPrograms[1].id); // White
      glDrawElements(GL_LINE_LOOP, numberOfIndices, GL_UNSIGNED_INT, 0);
      glUseProgram(shaderPrograms[2].id); // Red
      glBindVertexArray(pizza1.VAOs[0]);
      glPointSize(10.0f);
      glDrawElements(GL_POINTS, numberOfIndices, GL_UNSIGNED_INT, 0);
      glPointSize(1.0f);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Cleanup
  glDeleteVertexArrays(2, pizza1.VAOs.data());
  glDeleteBuffers(2, pizza1.VBOs.data());
  glDeleteBuffers(2, pizza1.EBOs.data());

  for (auto& program : shaderPrograms) {
    glDeleteShader(program.vertexShader);
    glDeleteShader(program.fragmentShader);
    glDeleteProgram(program.id);
  }

  glfwTerminate();
  return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (key == GLFW_KEY_D && action == GLFW_PRESS) {
    pizza1 *= moveRight;
  }
  if (key == GLFW_KEY_A && action == GLFW_PRESS) {
    pizza1 *= moveLeft;
  }
  if (key == GLFW_KEY_W && action == GLFW_PRESS) {
    pizza1 *= moveUp;
  }
  if (key == GLFW_KEY_S && action == GLFW_PRESS) {
    pizza1 *= moveDown;
  }
  if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
    renderMode = GL_TRIANGLES;
  }
  if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
    renderMode = GL_LINES;
  }
  if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
    renderMode = GL_POINTS;
  }
  if (key == GLFW_KEY_Z && action == GLFW_PRESS) {  
    pizza1.rotate_around_center(rotateZ);
  }
  if (key == GLFW_KEY_X && action == GLFW_PRESS) {
    pizza1.rotate_around_center(rotateZ_);
  }
  if (key == GLFW_KEY_E && action == GLFW_PRESS) {
    pizza1.scale_around_center(scaleUp);
  }
  if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
    pizza1.scale_around_center(scaleDown);
  }
  if (key == GLFW_KEY_O && action == GLFW_PRESS) {
    pizza1 *= moveRight;
    pizza1.scale_around_center(scaleDown);
    pizza1.rotate_around_center(rotateZ);
  }
  if (key == GLFW_KEY_L && action == GLFW_PRESS) {
    pizza1 *= moveDown;
    pizza1.scale_around_center(scaleUp);
    pizza1.rotate_around_center(rotateZ);
  }
  if (key == GLFW_KEY_K && action == GLFW_PRESS) {
    pizza1 *= moveLeft;
    pizza1.scale_around_center(scaleDown);
    pizza1.rotate_around_center(rotateZ);
  }
  if (key == GLFW_KEY_I && action == GLFW_PRESS) {
    pizza1 *= moveUp;
    pizza1.scale_around_center(scaleUp);
    pizza1.rotate_around_center(rotateZ);
  }
}