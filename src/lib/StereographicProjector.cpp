#include <vector>
#include <cmath>

#include "StereographicProjector.hpp"
#include "point.hpp"

CartesianPoint StereographicProjector::projectToSphere(Point2D &point)
{
  CartesianPoint outPoint;
  
  float denominator = ( 1 + point.x * point.x + point.y * point.y);
  outPoint.x = (2 * point.x) / denominator;
  outPoint.y = (2 * point.y) / denominator;
  outPoint.z = (-1 + point.x * point.x + point.y * point.y) / (2 * denominator);
 
  return outPoint;
} // StereographicProjector::projectToSphere

Point2D StereographicProjector::projectToPlane(CartesianPoint &point)
{
  Point2D outPoint;
  outPoint.x = point.x / (1 - point.z);
  outPoint.y = point.y / (1 - point.z);
  return outPoint;
}

