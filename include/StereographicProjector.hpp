#ifndef STEREOGRAPHICPROJECTOR_H
#define STEREOGRAPHICPROJECTOR_H
#include "AbstractProjector.hpp"

class StereographicProjector : public AbstractProjector
{
  public:
  StereographicProjector(double radius);
  
  CartesianPoint projectToSphere(Point2D &point) const override;

  Point2D projectToPlane(CartesianPoint &point) const override;
};

#endif
