#ifndef POINT_HPP
#define POINT_HPP


template <typename Scale_t>
struct Point2D {

    Scale_t x_;
    Scale_t y_;

    Point2D()                     : x_(0), y_(0) {}
    Point2D(Scale_t x, Scale_t y) : x_(x), y_(y) {}
    Point2D(const Point2D& other) : x_(other.x_),
                                    y_(other.y_) {}

    Point2D& operator=(const Point2D& other) {
      x_ = other.x_;
      y_ = other.y_;
      return *this;
    }

#include "point_operators.gen.inl"

};


#endif // POINT_HPP

