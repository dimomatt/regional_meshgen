#ifndef HELPERS_H
#define HELPERS_H
#include "point.hpp"
#include <set>
#include <vector>
#include <cmath>

Point2D circumcenter2D(LatLonPoint &a,
                       LatLonPoint &b,
                       LatLonPoint &c);


Point2D circumcenter2D(Point2D &a,
                       Point2D &b,
                       Point2D &c){
  Point2D outPoint;
  double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
  outPoint.x = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + (b.x * b.x + b.y * b.y) * (c.y - a.y)
               + (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
  outPoint.y = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + (b.x * b.x + b.y * b.y) * (a.x - c.x)
               + (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;
  return outPoint;
}

Point2D midpoint(Point2D &a, Point2D &b){
  Point2D midpoint;
  midpoint.x = (a.x + b.x) / 2.0;
  midpoint.y = (a.y + b.y) / 2.0;
  return midpoint;
}

double distanceSquared(Point2D &a, Point2D &b){
  double dx = b.x - a.x;
  double dy = b.y - a.y;
  return (dx + dy);
}

double distance(Point2D &a, Point2D &b){
  return sqrt(distanceSquared(a, b));
}

double kiteArea(double p, double q){
  return (p * q ) / 2;
}

double kiteArea(double a, double b, double theta){
  return (a * b) * sin(theta); 
}


double getAngle(const Point2D& a, const Point2D &b){
  double angle = std::atan2(b.y - a.y, b.x - a.x);
  return angle < 0 ? angle + 2*M_PI : angle;
}

template <typename T>
std::vector<std::vector<T>> convertSetsToVectors(
    std::vector<std::set<T>> &input){
  std::vector<std::vector<T>> output(input.size());
  for (size_t i=0; i < input.size(); i++){
    output[i] = std::vector<T>(input[i].begin(), input[i].end());
  }
  return output;
}

#endif //HELPERS_H
