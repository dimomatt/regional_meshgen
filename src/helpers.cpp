#include "helpers.hpp"
#include <cmath>
Point2D circumcenter2D(Point2D &a, Point2D &b, Point2D &c){
  Point2D outPoint;
  double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
  outPoint.x = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + (b.x * b.x + b.y * b.y) * (c.y - a.y)
               + (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
  outPoint.y = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + (b.x * b.x + b.y * b.y) * (a.x - c.x)
               + (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;
  return outPoint;
} 

double kiteArea(double p, double q){
  return (p * q ) / 2;
}

double kiteArea(double a, double b, double theta){
  return (a * b) * sin(theta); 
}


