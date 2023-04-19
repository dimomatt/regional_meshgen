#include <iostream>
#include "StereographicProjector.hpp"
#include "RegularMesh.hpp"

int main(int argc, const char **argv)
{
  RegularMesh mesh = RegularMesh(10, 10, 1.0);
  mesh.generateCells();
}
