#include <cstdio>
#include "3D/segment.hpp"
#include "3D/distance.hpp"


template <typename Scale_t>
int scanf(const char* format, Point3D<Scale_t>& point) {
  scanf(format, &point.x_);
  scanf(format, &point.y_);
  scanf(format, &point.z_);
  return 0;
}


int main() {
  Segment3D<double> segm1;
  Segment3D<double> segm2;
  scanf("%lf", segm1.begin_);
  scanf("%lf", segm1.end_);
  scanf("%lf", segm2.begin_);
  scanf("%lf", segm2.end_);

  printf("%.6f", GetDistance(segm1, segm2));

  return 0;
}

