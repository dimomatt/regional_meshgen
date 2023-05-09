#ifndef STEREOGRAPHICPROJECTOR_H
#define STEREOGRAPHICPROJECTOR_H
#include "AbstractProjector.hpp"

class StereographicProjector : public AbstractProjector
{
  public:
  StereographicProjector(double radius);
  
  CartesianPoint projectToSphere(Point2D &point);

  Point2D projectToPlane(CartesianPoint &point);
};

#endif
