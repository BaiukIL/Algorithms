#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>
#include "point.hpp"


template <typename Scale_t>
struct Vector3D;

template <typename Scale_t>
Vector3D<Scale_t> CrossProduct(const Vector3D<Scale_t>& lhs,
                               const Vector3D<Scale_t>& rhs) {
  return Vector3D<Scale_t>(lhs.y_*rhs.z_ - lhs.z_*rhs.y_,
                           lhs.z_*rhs.x_ - lhs.x_*rhs.z_,
                           lhs.x_*rhs.y_ - lhs.y_*rhs.x_);
}

template <typename Scale_t>
Scale_t DotProduct(const Vector3D<Scale_t>& lhs,
                   const Vector3D<Scale_t>& rhs) {
  return lhs.x_*rhs.x_ + lhs.y_*rhs.y_ + lhs.z_*rhs.z_;
}


template <typename Scale_t>
struct Vector3D : Point3D<Scale_t> {

  Vector3D()
      : Point3D<Scale_t>() {}

  Vector3D(Scale_t x, Scale_t y, Scale_t z)
      : Point3D<Scale_t>(x, y, z) {}

  Vector3D(const Point3D<Scale_t>& point)
      : Point3D<Scale_t>(point) {}

  Vector3D(const Point3D<Scale_t>& begin, const Point3D<Scale_t>& end)
      : Point3D<Scale_t>(end.x_ - begin.x_,
                         end.y_ - begin.y_,
                         end.z_ - begin.z_) {}

  Vector3D(const Vector3D<Scale_t>& other)
      : Point3D<Scale_t>(other.x_, other.y_, other.z_) {}

  Vector3D& operator=(const Vector3D<Scale_t>& other) {
    this->x_ = other.x_;
    this->y_ = other.y_;
    this->z_ = other.z_;
    return *this;
  }

  Vector3D& operator=(const Point3D<Scale_t>& point) {
    this->x_ = point.x_;
    this->y_ = point.y_;
    this->z_ = point.z_;
    return *this;
  }

  void Assign(const Point3D<Scale_t>& begin, const Point3D<Scale_t>& end) {
    this->x_ = end.x_ - begin.x_;
    this->y_ = end.y_ - begin.y_;
    this->z_ = end.z_ - begin.z_;
  }
};


#endif // VECTOR_HPP

