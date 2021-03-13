#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <cmath>
#include "point.hpp"


template <typename Scale_t>
struct Vector2D;

template <typename Scale_t>
Scale_t CrossProduct(const Vector2D<Scale_t>& lhs,
                     const Vector2D<Scale_t>& rhs) {
  return lhs.x_*rhs.y_ - lhs.y_*rhs.x_;
}

template <typename Scale_t>
Scale_t DotProduct(const Vector2D<Scale_t>& lhs,
                   const Vector2D<Scale_t>& rhs) {
  return lhs.x_*rhs.x_ + lhs.y_*rhs.y_;
}

/// Return angle between vector and ox axis.
template <typename Scale_t>
double GetOXAngle(const Vector2D<Scale_t>& vector) {
  Scale_t x = vector.x_;
  Scale_t y = vector.y_;

  if (y >= 0) {
    return atan2(y, x);
  }
  return atan2(y, x) + 2 * M_PI;
}


template <typename Scale_t>
struct Vector2D : Point2D<Scale_t> {

  Vector2D()
      : Point2D<Scale_t>() {}

  Vector2D(const Point2D<Scale_t>& point)
      : Point2D<Scale_t>(point) {}

  Vector2D(const Point2D<Scale_t>& begin, const Point2D<Scale_t>& end)
      : Point2D<Scale_t>(end.x_ - begin.x_, end.y_ - begin.y_) {}

  Vector2D(const Vector2D<Scale_t>& other)
      : Point2D<Scale_t>(other.x_, other.y_) {}

  Vector2D& operator=(const Vector2D<Scale_t>& other) {
    this->x_ = other.x_;
    this->y_ = other.y_;
    return *this;
  }

  Vector2D& operator=(const Point2D<Scale_t>& point) {
    this->x_ = point.x_;
    this->y_ = point.y_;
    return *this;
  }

  void Assign(const Point2D<Scale_t>& begin, const Point2D<Scale_t>& end) {
    this->x_ = end.x_ - begin.x_;
    this->y_ = end.y_ - begin.y_;
  }
};


#endif // SEGMENT_HPP

