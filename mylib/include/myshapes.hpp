#ifndef MYSHAPES_HPP
#define MYSHAPES_HPP

#include "mylib.hpp"
#include <vector>

ogl::translation moveRight(0.1f, 0.0f, 0.0f);
ogl::translation moveLeft(-0.1f, 0.0f, 0.0f);
ogl::translation moveUp(0.0f, 0.1f, 0.0f);
ogl::translation moveDown(0.0f, -0.1f, 0.0f);
ogl::scale scaleDown(0.90f, 0.90f, 1.0f);
ogl::scale scaleUp(1.1f, 1.1f, 1.0f);
ogl::rotate_z rotateZ(0.1f);
ogl::rotate_z rotateZ_(-0.1f);

ogl::shape testShape;
ogl::shape pizza1;

void create_testShape() {
  testShape.add_vertex(ogl::point(-0.5f, -0.5f, 0.0f));
  testShape.add_vertex(ogl::point(0.5f, -0.5f, 0.0f));
  testShape.add_vertex(ogl::point(0.0f, 0.5f, 0.0f));
  testShape.add_index(std::vector<unsigned int>{0, 1, 2});
  testShape.update_center();
}

void create_pizza1() {
  pizza1.add_vertex(ogl::point( 0.00f, 0.50f, 0.0f));
  pizza1.add_vertex(ogl::point( 0.37f, 0.37f, 0.0f));
  pizza1.add_vertex(ogl::point( 0.50f, 0.00f, 0.0f));
  pizza1.add_vertex(ogl::point( 0.37f,-0.37f, 0.0f));
  pizza1.add_vertex(ogl::point( 0.00f,-0.50f, 0.0f));
  pizza1.add_vertex(ogl::point(-0.37f,-0.37f, 0.0f));
  pizza1.add_vertex(ogl::point(-0.50f, 0.00f, 0.0f));
  pizza1.add_vertex(ogl::point(-0.37f, 0.37f, 0.0f));
  pizza1.add_vertex(ogl::point( 0.00f, 0.00f, 0.0f));

  // Indices for lines (outline and spokes)
  pizza1.add_index(std::vector<unsigned int>{
    0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 0,
    0, 8, 1, 8, 2, 8, 3, 8, 4, 8, 5, 8, 6, 8, 7, 8
  });

  // Indices for triangles (filled pizza)
  pizza1.add_index(std::vector<unsigned int>{
    0, 1, 8,
    1, 2, 8,
    2, 3, 8,
    3, 4, 8,
    4, 5, 8,
    5, 6, 8,
    6, 7, 8,
    7, 0, 8
  });

  pizza1.VAOs.resize(1);
  pizza1.VBOs.resize(1);
  pizza1.EBOs.resize(2);

  pizza1.update_center();
}

#endif