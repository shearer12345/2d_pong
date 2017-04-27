#pragma once
#include "Mesh.hpp"

class Circle : public Mesh {
public:
  Circle(GLSLProgram *glslProgram, int segments = 5);
  int segments;
};
