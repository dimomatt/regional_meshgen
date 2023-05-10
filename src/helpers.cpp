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


std::vector<std::vector<int>> convertSetsToVectors(
    std::vector<std::set<int>> &input){
  std::vector<std::vector<int>> output;
  output.resize(input.size());
  for (size_t i = 0; i < input.size(); i++){
    output[i] = std::vector<int>(input[i].begin(), input[i].end());
  }
  return output;
}

double angle(const Point2D& a, const Point2D&b){
  return std::atan2(a.y - b.y, a.x - b.x);
}
