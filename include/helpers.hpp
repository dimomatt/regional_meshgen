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

Point2D midpoint(const Point2D &a, const Point2D &b);

double angle(const Point2D& a, const Point2D &b);

double distanceSquared(const Point2D &a, const Point2D &b);

double distance(const Point2D &a, const Point2D &b);

double kiteArea(const double p, const double q);

double kiteArea(const double a, const double b, const double theta);

// This really should be templated
std::vector<std::vector<int>> convertSetsToVectors(
    std::vector<std::set<int>> &input);

#endif //HELPERS_H
