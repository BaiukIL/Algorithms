#ifndef POINT_HPP
#define POINT_HPP


template <typename Scale_t>
struct Point3D {

    Scale_t x_;
    Scale_t y_;
    Scale_t z_;

    Point3D()                                : x_(0), y_(0), z_(0) {}
    Point3D(Scale_t x, Scale_t y, Scale_t z) : x_(x), y_(y), z_(z) {}
    Point3D(const Point3D& other)            : x_(other.x_),
                                               y_(other.y_),
                                               z_(other.z_) {}

    Point3D& operator=(const Point3D& other) {
      x_ = other.x_;
      y_ = other.y_;
      z_ = other.z_;
      return *this;
    }

#include "point_operators.gen.inl"

};



#endif // POINT_HPP

