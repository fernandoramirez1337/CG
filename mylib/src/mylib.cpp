#include "mylib.hpp"
#include "shaders.hpp"
#include <glad/glad.h>


#include <cmath>
#include <numeric>
#include <algorithm>


// ------------------------------
// vector class
// ------------------------------
float ogl::vector::length() const noexcept {
  return std::sqrt(x*x + y*y + z*z);
}

ogl::vector ogl::vector::normalize() const noexcept {
  float len = length();
  return (len != 0.0f) ? ogl::vector(x / len, y / len, z / len) : *this;
}

float ogl::vector::dot(const vector& v) const noexcept {
  return x * v.x + y * v.y + z * v.z;
}

ogl::vector ogl::vector::cross(const vector& v) const noexcept {
  return ogl::vector(
    y * v.z - z * v.y,
    z * v.x - x * v.z,
    x * v.y - y * v.x
  );
}

// ------------------------------
// point class
// ------------------------------
// Remove this implementation as it's defaulted in the header
// ogl::point& ogl::point::operator=(const ogl::point& other) noexcept {
//   if (this != &other) {
//     x = other.x;
//     y = other.y;
//     z = other.z;
//   }
//   return *this;
// }

// ------------------------------
// matrix class
// ------------------------------
ogl::matrix::matrix() noexcept {
  set_identity();
}

[[nodiscard]] ogl::matrix ogl::matrix::operator*(const ogl::matrix& mat) const noexcept {
  ogl::matrix result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = 0.0f;
      for (int k = 0; k < 4; ++k) {
        result.m[i][j] += m[i][k] * mat.m[k][j];
      }
    }
  }
  return result;
}

ogl::matrix& ogl::matrix::operator*=(const ogl::matrix& mat) noexcept {
  *this = *this * mat;
  return *this;
}

[[nodiscard]] ogl::point ogl::matrix::operator*(const ogl::point& poi) const noexcept {
  float x[4] = {poi.x, poi.y, poi.z, 1.0f};
  float result[4] = {0.0f};

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result[i] += m[i][j] * x[j];
    }
  }

  float w_inv = (result[3] != 0.0f) ? 1.0f / result[3] : 1.0f;
  return ogl::point(result[0] * w_inv, result[1] * w_inv, result[2] * w_inv);
}

[[nodiscard]] ogl::matrix ogl::matrix::transpose() const noexcept {
  ogl::matrix result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.m[i][j] = m[j][i];
    }
  }
  return result;
}

void ogl::matrix::set_identity() noexcept {
  m = {{
    {1.0f, 0.0f, 0.0f, 0.0f},
    {0.0f, 1.0f, 0.0f, 0.0f},
    {0.0f, 0.0f, 1.0f, 0.0f},
    {0.0f, 0.0f, 0.0f, 1.0f}
  }};
}

[[nodiscard]] const ogl::matrix4& ogl::matrix::get() const noexcept {
  return m;
}

void ogl::matrix::set(const ogl::matrix4& mat) noexcept {
  m = mat;
}

// ------------------------------
// Scale matrix
// ------------------------------
ogl::scale::scale(float sx, float sy, float sz) noexcept : matrix() {
  m[0][0] = sx;
  m[1][1] = sy;
  m[2][2] = sz;
}

// ------------------------------
// Rotation matrices
// ------------------------------
ogl::rotate_x::rotate_x(float angle) noexcept : matrix() {
  float cos_a = std::cos(angle);
  float sin_a = std::sin(angle);

  m[1][1] = cos_a;
  m[1][2] = -sin_a;
  m[2][1] = sin_a;
  m[2][2] = cos_a;
}

ogl::rotate_y::rotate_y(float angle) noexcept : matrix() {
  float cos_a = std::cos(angle);
  float sin_a = std::sin(angle);

  m[0][0] = cos_a;
  m[0][2] = sin_a;
  m[2][0] = -sin_a;
  m[2][2] = cos_a;
}

ogl::rotate_z::rotate_z(float angle) noexcept : matrix() {
  float cos_a = std::cos(angle);
  float sin_a = std::sin(angle);

  m[0][0] = cos_a;
  m[0][1] = -sin_a;
  m[1][0] = sin_a;
  m[1][1] = cos_a;
}

// ------------------------------
// Translation matrix
// ------------------------------
ogl::translation::translation(float tx, float ty, float tz) noexcept : matrix() {
  m[0][3] = tx;
  m[1][3] = ty;
  m[2][3] = tz;
}

// ------------------------------
// Projection matrix
// ------------------------------
ogl::projection::projection(float fov, float aspect, float near, float far) noexcept : matrix() {
  float fov_rad = fov * M_PI / 180.0f;
  float tan_half_fov = std::tan(fov_rad / 2.0f);
  float range = far - near;

  m[0][0] = 1.0f / (aspect * tan_half_fov);
  m[1][1] = 1.0f / tan_half_fov;
  m[2][2] = -(far + near) / range;
  m[2][3] = -2.0f * far * near / range;
  m[3][2] = -1.0f;
  m[3][3] = 0.0f;
}

// ------------------------------
// Shape class
// ------------------------------
ogl::shape::shape() : center(0.0f, 0.0f, 0.0f) {}

ogl::shape::~shape() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(indices.size(), &EBOs[0]);
}

ogl::shape::shape(shape&& other) noexcept
  : vertices(std::move(other.vertices)),
    center(std::move(other.center)),
    indices(std::move(other.indices)) {}

ogl::shape& ogl::shape::operator=(shape&& other) noexcept {
  if (this != &other) {
    vertices = std::move(other.vertices);
    center = std::move(other.center);
    indices = std::move(other.indices);
  }
  return *this;
}

[[nodiscard]] std::vector<float> ogl::shape::get_vertices() const {
  std::vector<float> result;
  result.reserve(vertices.size() * 3);
  for (const auto& vertex : vertices) {
    result.push_back(vertex.x);
    result.push_back(vertex.y);
    result.push_back(vertex.z);
  }
  return result;
}

[[nodiscard]] std::vector<unsigned int> ogl::shape::get_indices(size_t base) const {
  return indices[base];
}

void ogl::shape::add_vertex(const ogl::point& v) {
  vertices.push_back(v);
}

void ogl::shape::add_index(const std::vector<unsigned int>& i) {
  indices.push_back(i);
}

ogl::shape& ogl::shape::operator*=(const matrix& transform) {
  for (auto& vertex : vertices) {
    vertex = transform * vertex;
  }
  center = transform * center;
  return *this;
}

[[nodiscard]] ogl::shape ogl::shape::operator*(const matrix& transform) const {
  shape result = *this;
  result *= transform;
  return result;
}

void ogl::shape::rotate_around_center(const ogl::rotate_z& rotation_matrix) {
  ogl::point currentCenter = center;
  *this *= ogl::translation(-center.x, -center.y, -center.z);
  *this *= rotation_matrix;
  *this *= ogl::translation(currentCenter.x, currentCenter.y, currentCenter.z);
  center = currentCenter;
}

void ogl::shape::update_center() {
  center = std::accumulate(vertices.begin(), vertices.end(), ogl::point(),
    [](const ogl::point& acc, const ogl::point& v) {
      return ogl::point(acc.x + v.x, acc.y + v.y, acc.z + v.z);
    });
  float inv_size = 1.0f / vertices.size();
  center.x *= inv_size;
  center.y *= inv_size;
  center.z *= inv_size;
}

void ogl::shape::scale_around_center(const ogl::scale& scale_matrix) {
  ogl::point currentCenter = center;
  *this *= ogl::translation(-center.x, -center.y, -center.z);
  *this *= scale_matrix;
  *this *= ogl::translation(currentCenter.x, currentCenter.y, currentCenter.z);
}

void ogl::shape::get_vertices(float* vertices_array) const {
  size_t i = 0;
  for (const auto& vertex : vertices) {
    vertices_array[i++] = vertex.x;
    vertices_array[i++] = vertex.y;
    vertices_array[i++] = vertex.z;
  }
}

void ogl::shape::get_indices(unsigned int* indices_array, size_t base) const {
  if (base < indices.size()) {
    std::copy(indices[base].begin(), indices[base].end(), indices_array);
  }
}

void ogl::shape::setup_gl() {
  size_t size_of_vertices = vertices.size() * sizeof(float) * COMPONENTS_PER_VERTEX;
  size_t how_many_EBOs = indices.size();
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(how_many_EBOs, &EBOs[0]);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, size_of_vertices, vertices.data(), GL_DYNAMIC_DRAW);

  for (size_t i = 0; i < how_many_EBOs; ++i) {
    size_t size_of_indices = indices[i].size() * sizeof(unsigned int);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_of_indices, indices[i].data(), GL_STATIC_DRAW);
  }

  glVertexAttribPointer(0, COMPONENTS_PER_VERTEX, GL_FLOAT, GL_FALSE, COMPONENTS_PER_VERTEX * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
}

void ogl::shape::sub_data_gl() {
  size_t size_of_vertices = vertices.size() * sizeof(float) * COMPONENTS_PER_VERTEX;  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size_of_vertices, vertices.data());
}

void ogl::shape::draw_gl(unsigned int render_mode,std::array<ShaderProgram, 5> shaderPrograms) {
  if (render_mode == GL_LINES) {
    glUseProgram(shaderPrograms[WHITE].id); // White
    glBindVertexArray(VAO);
    glDrawElements(GL_LINE_LOOP, indices[0].size() , GL_UNSIGNED_INT, 0);
  } else if (render_mode == GL_TRIANGLES) {
    glUseProgram(shaderPrograms[BLACK].id); // Black
    glBindVertexArray(VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
    glDrawElements(GL_TRIANGLES, indices[1].size(), GL_UNSIGNED_INT, 0);
  } else if (render_mode == GL_POINTS) {
    glUseProgram(shaderPrograms[WHITE].id); // White
    glDrawElements(GL_LINE_LOOP, indices[0].size(), GL_UNSIGNED_INT, 0);
    glUseProgram(shaderPrograms[RED].id); // Red
    glBindVertexArray(VAO);
    glPointSize(10.0f);
    glDrawElements(GL_POINTS, indices[0].size(), GL_UNSIGNED_INT, 0);
    glPointSize(1.0f);
  }
}