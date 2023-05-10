#include <vector>
#include <cmath>

#include "StereographicProjector.hpp"
#include "point.hpp"

StereographicProjector::StereographicProjector(double radius){
  this->radius = radius;
}

CartesianPoint StereographicProjector::projectToSphere(Point2D &point)
const {
  CartesianPoint outPoint;
  
  float denominator = ( this->radius * this->radius + 
                        point.x * point.x + 
                        point.y * point.y);
  outPoint.x = this->radius * ((2 * point.x) / denominator);
  outPoint.y = this->radius * ((2 * point.y) / denominator);
  outPoint.z = this->radius * ((this->radius * this->radius - 
                                point.x * point.x - 
                                point.y * point.y) / 
                                (2 * denominator));
 
  return outPoint;
}; // StereographicProjector::projectToSphere

Point2D StereographicProjector::projectToPlane(CartesianPoint &point)
const {
  Point2D outPoint;
  outPoint.x = this->radius * (point.x / (this->radius - point.z));
  outPoint.y = this->radius * (point.y / (this->radius - point.z));
  return outPoint;
};

