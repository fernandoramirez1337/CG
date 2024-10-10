#ifndef OGL_MYLIB_HPP
#define OGL_MYLIB_HPP

#include <array>
#include <cmath>
#include <vector>

namespace ogl {

class vector {
public:
  float x, y, z;

  constexpr vector(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) noexcept : x(x_), y(y_), z(z_) {}
  constexpr vector(const vector& v) noexcept = default;
  ~vector() = default;

  [[nodiscard]] float length() const noexcept;
  [[nodiscard]] vector normalize() const noexcept;
  [[nodiscard]] float dot(const vector& v) const noexcept;
  [[nodiscard]] vector cross(const vector& v) const noexcept;
};

class point {
public:
  float x, y, z;

  constexpr point(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f) noexcept : x(x_), y(y_), z(z_) {}
  constexpr point(const point& other) noexcept = default;
  ~point() = default;

  point& operator=(const point& other) = default;
};

using matrix4 = std::array<std::array<float, 4>, 4>;

class matrix {
protected:
  matrix4 m;

public:
  matrix() noexcept;

  [[nodiscard]] matrix operator*(const matrix& mat) const noexcept;
  matrix& operator*=(const matrix& mat) noexcept;
  [[nodiscard]] point operator*(const point& poi) const noexcept;

  void set_identity() noexcept;
  [[nodiscard]] const matrix4& get() const noexcept;
  void set(const matrix4& mat) noexcept;
  [[nodiscard]] matrix transpose() const noexcept;
};

class scale : public matrix {
public:
  scale(float sx, float sy, float sz) noexcept;
};

class rotate_x : public matrix {
public:
  explicit rotate_x(float angle) noexcept;
};

class rotate_y : public matrix {
public:
  explicit rotate_y(float angle) noexcept;
};

class rotate_z : public matrix {
public:
  explicit rotate_z(float angle) noexcept;
};

class translation : public matrix {
public:
  translation(float tx, float ty, float tz) noexcept;
};

class projection : public matrix {
public:
  projection(float fov, float aspect, float near, float far) noexcept;
};

class shape {
public:
  std::vector<ogl::point> vertices;
  ogl::point center;
  std::vector<std::vector<unsigned int>> indices;

  std::vector<unsigned int> EBOs;
  std::vector<unsigned int> VAOs;
  std::vector<unsigned int> VBOs;

  shape();
  ~shape();

  void add_vertex(const ogl::point&);
  void add_index(const std::vector<unsigned int>&);

  void get_vertices(float*) const;
  void get_indices(unsigned int*, size_t = 0) const;

  shape& operator*=(const matrix& transform) {
    for (auto& vertex : vertices) {
      vertex = transform * vertex;
    }
    center = transform * center;
    return *this;
  }

  [[nodiscard]] shape operator*(const matrix& transform) const {
    shape result = *this;
    result *= transform;
    return result;
  }

  void rotate_around_center(const ogl::rotate_z& rotation_matrix) {
    // Store the current center
    ogl::point currentCenter = center;

    // Translate to origin
    *this *= ogl::translation(-center.x, -center.y, -center.z);
    
    // Apply rotation
    *this *= rotation_matrix;
    
    *this *= ogl::translation(currentCenter.x, currentCenter.y, currentCenter.z);

    center = currentCenter;
  }

  void update_center() {
    center = ogl::point(0.0f, 0.0f, 0.0f);
    for (const auto& vertex : vertices) {
      center.x += vertex.x;
      center.y += vertex.y;
      center.z += vertex.z;
    }
    center.x /= vertices.size();
    center.y /= vertices.size();
    center.z /= vertices.size();
  }

  void scale_around_center(const ogl::scale& scale_matrix) {
    ogl::point currentCenter = center;

    *this *= ogl::translation(-center.x, -center.y, -center.z);
    *this *= scale_matrix;
    *this *= ogl::translation(currentCenter.x, currentCenter.y, currentCenter.z);

  }
};

} // namespace ogl

#endif // OGL_MYLIB_HPP