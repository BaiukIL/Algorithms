#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <vector>
#include "point.hpp"


template<typename Scale_t>
struct Polygon {

  std::vector<Point2D<Scale_t>> points_;

  Polygon(size_t size)
      : points_(size) {}

  Polygon(const std::vector<Point2D<Scale_t>>& points)
      : points_(points) {}

  Polygon(std::vector<Point2D<Scale_t>>&& points)
      : points_(std::move(points)) {}

  Polygon(const Polygon& other)
      : points_(other.points_) {}

  size_t Size() const { return points_.size(); }
};


#endif // POLYGON_HPP

