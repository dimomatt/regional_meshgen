#ifndef HELPERS_H
#define HELPERS_H
#include "point.hpp"
#include <set>
#include <vector>

Point2D circumcenter2D(Point2D &a,
                       Point2D &b,
                       Point2D &c);

Point2D circumcenter2D(LatLonPoint &a,
                       LatLonPoint &b,
                       LatLonPoint &c);

Point2D midpoint(Point2D &a, Point2D &b);

double distanceSquared(Point2D &a, Point2D &b);

double distance(Point2D &a, Point2D &b);

double kiteArea(double p, double q);

double kiteArea(double a, double b, double theta);

// This really should be templated
std::vector<std::vector<int>> convertSetsToVectors(
    std::vector<std::set<int>> &input);

#endif //HELPERS_H
