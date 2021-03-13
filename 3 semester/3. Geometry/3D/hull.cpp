#include "hull.hpp"
#include <cmath>


Point3D<double> Hull::nil_point = Point3D<double>(INF, INF, INF);
HullPoint       Hull::nil       = HullPoint(nil_point);
HullPoint*      Hull::NIL       = &nil;

/// Point rotations in X, Y and Z axises by given angle.
// See https://en.wikipedia.org/wiki/Rotation_matrix
// 'In three dimensions' section.
inline void RotateX(Point3D<double>& point, double angle) {
  double z =  point.z_ * std::cos(angle) + point.y_ * std::sin(angle);
  double y = -point.z_ * std::sin(angle) + point.y_ * std::cos(angle);
  point.z_ = z;
  point.y_ = y;
}

inline void RotateY(Point3D<double>& point, double angle) {
  double x =  point.x_ * std::cos(angle) + point.z_ * std::sin(angle);
  double z = -point.x_ * std::sin(angle) + point.z_ * std::cos(angle);
  point.x_ = x;
  point.z_ = z;
}

inline void RotateZ(Point3D<double>& point, double angle) {
  double x =  point.x_ * std::cos(angle) + point.y_ * std::sin(angle);
  double y = -point.x_ * std::sin(angle) + point.y_ * std::cos(angle);
  point.x_ = x;
  point.y_ = y;
}

Hull::Hull(std::vector<Point3D<double>>& points,
           bool build_facets,
           bool build_projection)

    : points_(),
      has_projection_(build_projection),
      projection_points_(),
      has_facets_(build_facets),
      facets_() {

  points_.reserve(points.size());

  if (build_facets) { facets_.reserve(points.size() * 4); }
  if (build_projection) { projection_points_.reserve(points.size()); }

  double angle = 0.000000001;
  for (Point3D<double>& point : points) {
    RotateX(point, angle); RotateY(point, angle); RotateZ(point, angle);
  }

  BuildFullHull(points);

  for (Point3D<double>& point : points) {
    RotateZ(point, -angle); RotateY(point, -angle); RotateX(point, -angle);
  }
}

void Hull::AddPoint(HullPoint* point) {
  points_.insert(point->prev_->point_);
  points_.insert(point->point_);
  points_.insert(point->next_->point_);
}

void Hull::AddFacet(HullPoint* point) {
  facets_.emplace_back(Facet<double>(point->prev_->point_,
                                     point->point_,
                                     point->next_->point_));
}

void Hull::BuildHalfProjection(HullPoint* list) {
  for (HullPoint* ptr = list; ptr != NIL; ptr = ptr->next_) {
    projection_points_.insert(ptr->point_);
  }
}


void Hull::BuildFullHull(std::vector<Point3D<double>>& points) {
  size_t points_num = points.size();
  HullPoint* hull_points = new HullPoint [points_num];
  // Construct HullPoint array.
  for (size_t i = 0; i < points.size(); ++i) {
    hull_points[i] = HullPoint(points[i]);
  }
  // Allocate memory for pointers. Note that we allocate double size array,
  // for, as Chan has mentioned: "each point is inserted at most once and
  // deleted at most once, so the total number of events is at most 2n".
  HullPoint** events = new HullPoint*[points_num * 2];
  HullPoint** tmp    = new HullPoint*[points_num * 2]; // Temporary array.

  // Build lower hull.
  HullPoint* list = Sort(hull_points, points_num);
  BuildLowerHull(list, points_num, events, tmp);

  // Build lower 2D convex hull (time t = -inf) if required.
  if (has_projection_) { BuildHalfProjection(list); }
  // Do not forget to rebuild hull by calling Act() events method.
  for (size_t i = 0; events[i] != NIL; events[i++]->Act()) {
    if (has_facets_) { AddFacet(events[i]); }
    AddPoint(events[i]);
  }
  // Build upper 2D convex hull (for time t = inf).
  if (has_projection_) { BuildHalfProjection(list); }

  // Invert points by z axis to build upper hull.
  for (size_t i = 0; i < points_num; ++i) { hull_points[i].point_->z_ *= -1; }

  // Build upper hull (note that z coordinates are reversed).
  list = Sort(hull_points, points_num);
  BuildLowerHull(list, points_num, events, tmp);

  // Fill facets.
  for (size_t i = 0; events[i] != NIL; events[i++]->Act()) {
    if (has_facets_) { AddFacet(events[i]); }
    AddPoint(events[i]);
  }

  // Fix z coordinates.
  for (size_t i = 0; i < points_num; ++i) { hull_points[i].point_->z_ *= -1; }

  delete[] events;
  delete[] tmp;
  delete[] hull_points;
}

double Hull::GetTurn(HullPoint* p, HullPoint* q, HullPoint* r) {
  // If any of points does not exist return any positive number
  // (for instance 1.0).
  if (p == NIL || q == NIL || r == NIL) { return 1.0; }
  // [p, q] and [p, r] vectors cross product.
  return (q->point_->x_ - p->point_->x_)*(r->point_->y_ - p->point_->y_) -
         (r->point_->x_ - p->point_->x_)*(q->point_->y_ - p->point_->y_);
}

double Hull::GetTime(HullPoint* p, HullPoint* q, HullPoint* r) {
  // If any of points does not exist then it is
  if (p == NIL || q == NIL || r == NIL) return INF;
  // Solve simultaneous equations:
  // (1) p->x + (p->z - t*p->y) = q->x + (q->z - t*q->y);
  // (2) p->x + (p->z - t*p->y) = r->x + (r->z - t*r->y);
  // and extract `t` which is desired time.
  // It is exactly an underlying formula.
  return ((q->point_->x_ - p->point_->x_)*(r->point_->z_ - p->point_->z_) -
          (r->point_->x_ - p->point_->x_)*(q->point_->z_ - p->point_->z_))
              / GetTurn(p, q, r);
}

HullPoint* Hull::Sort(HullPoint* points, size_t n) {

  if (n == 1) {
    points->next_ = NIL;
    return points;
  }
  HullPoint* first  = Sort(points, n/2);
  HullPoint* second = Sort(points + n/2, n - n/2);
  HullPoint  head; // Hack: create element on stack and initialize
  // `current` pointer with it instead of calling heap allocation functions.
  HullPoint* current = &head;
  // Use do {} while() unstead of while(), for first assignment is neccessary.
  do {
    if (first->point_->x_ <= second->point_->x_) {
      current->next_ = first;
      current = current->next_;
      first = first->next_;
    } else {
      current->next_ = second;
      current = current->next_;
      second = second->next_;
    }
  } while (current != NIL);
  return head.next_; // First valid pointer is stored in head.next.
}

void Insert(HullPoint** events, size_t pos, HullPoint* point,
            HullPoint** inserted, size_t inserted_size) {
  bool handled = false;
  for (size_t i = 0; i < inserted_size; ++i) {
    if (point == inserted[i]) {
      handled = true;
    }
  }
  if (!handled) {
    inserted[inserted_size] = events[pos] = point;
  }
}

void Hull::BuildLowerHull(HullPoint*  list, // List of x-sorted points.
                          size_t n,         // Number of points.
                          HullPoint** events,
                          HullPoint** tmp) {
  // Recursion tail.
  if (n == 1) {
    // Cut a vertex off the chain. Assign events[0] with NIL, because
    // no events has been made.
    *events = list->prev_ = list->next_ = NIL;
    return;
  }

  HullPoint* u = list; // The most right vertex of left hull.
  HullPoint* v = list; // The most left vertex of right hull.
  HullPoint* middle = list;

  // Seek for (middle) points with (n/2 - 1) and (n/2) indexes in the list.
  for (size_t i = 0; i < n/2 - 1; ++i) { u = u->next_; }
  middle = v = u->next_;

  // Build left and right hulls each of n/2 vertices.
  // Note that tmp and A are swaped, so hull is located in tmp.
  BuildLowerHull(list, n/2, tmp, events);
  BuildLowerHull(middle, n - n/2, tmp + n/2*2, events + n/2*2);
  // NOT 2*n/2 --------------------------^ !!!

  // Find initial bridge.
  bool bridge_found = false;
  while(!bridge_found) {
    // If u-uv is left triple, move u left.
    if (GetTurn(u->prev_, u, v) < 0)      { u = u->prev_; }
    // If uvv+ is left triple, move v right.
    else if (GetTurn(u, v, v->next_) < 0) { v = v->next_; }
    // If both conditions are true, then bridge is found.
    else bridge_found = true;
  }

  // Time array. Used to simplify code.
  double time[6];
  // Last event position (for full hull).
  size_t pos = 0;
  // l (resp. r) is an index of vertex with the least insertion time in
  // left (resp. right) hull.
  size_t l = 0, r = n/2*2;
  // old_time is a time of last handled hill change,
  // new_time is a time of next (not handled) hull change.
  double old_time = -INF, new_time = INF;
  // Merge loop.
  // Note that tmp (which is correct convex hull) is a time-sorted
  // SEQUENCE OF MOVIES (i.e. vertices are sorted by time of their insertion
  // in appropriate 3D hull for tmp).
  // It means, for instance, that GetTime(tmp[0]->prev_, tmp[0], tmp[0]->next_)
  // is a time of first insertion (tmp[0] vertex was inserted) in 3D hull
  // corresponding to tmp array.
  while (true) {
    // Order is important!
    // Time of next change in left hull.
    time[0] = GetTime(tmp[l]->prev_, tmp[l], tmp[l]->next_);
    // Time of next change in right hull.
    time[1] = GetTime(tmp[r]->prev_, tmp[r], tmp[r]->next_);
    // Times of all possible bridge states between left and right hulls during
    // their relative shift.
    time[2] = GetTime(u, u->next_, v);
    time[3] = GetTime(u->prev_, u, v);
    time[4] = GetTime(u, v->prev_, v);
    time[5] = GetTime(u, v, v->next_);

    // Find minimum time greater that old_time.
    new_time = INF;
    for (size_t i = 0; i < 6; ++i) {
      if (old_time < time[i] && time[i] < new_time) {
        new_time = time[i];
      }
    }
    // If nothing changes (i.e. if new_time == INF) build is finished.
    if (new_time == INF) { break; }
    // Array of inserted events.
    HullPoint* inserted[6];
    size_t inserted_size = 0;

    for (size_t time_id = 0; time_id < 6; ++time_id) {
      if (time[time_id] != new_time) { continue; }
      switch(time_id) {
        case 0:
          // If current event (which represents left convex hull change) lies
          // above bridge (i.e. point we want to insert turns out to be inside
          // current hull), do not insert point in result convex hull (events).
          if (tmp[l]->point_->x_ < u->point_->x_) {
            Insert(events, pos++, tmp[l], inserted, inserted_size++);
          }
          tmp[l++]->Act();
          break;
        case 1:
          // Similar to case 0.
          if (v->point_->x_ < tmp[r]->point_->x_) {
            Insert(events, pos++, tmp[r], inserted, inserted_size++);
          }
          tmp[r++]->Act();
          break;
        case 2: // u+ insertion. u+v is bridge.
          u = u->next_;
          Insert(events, pos++, u, inserted, inserted_size++);
          break;
        case 3: // u deletion. u-v is bridge.
          Insert(events, pos++, u, inserted, inserted_size++);
          u = u->prev_;
          break;
        case 4: // v- insertion. uv- is bridge.
          v = v->prev_;
          Insert(events, pos++, v, inserted, inserted_size++);
          break;
        case 5: // v deletion. uv+ is bridge.
          Insert(events, pos++, v, inserted, inserted_size++);
          v = v->next_;
          break;
      }
    }
    old_time = new_time; // Update time.
  }
  // NIL as an array end sign.
  events[pos] = NIL;
  // Fix bridge.
  u->next_ = v; v->prev_ = u;
  // Go back in time and update pointers.
  for (int i = pos - 1; i >= 0; --i) {
    if (events[i]->point_->x_ <= u->point_->x_ ||
        events[i]->point_->x_ >= v->point_->x_) {

      events[i]->Act();

      if (events[i] == u) {
        u = u->prev_;
      } else if (events[i] == v) {
        v = v->next_;
      }
    }
    else {
      u->next_ = events[i];
      events[i]->prev_ = u;
      v->prev_ = events[i];
      events[i]->next_ = v;

      if (events[i]->point_->x_ < middle->point_->x_) {
        u = events[i];
      } else {
        v = events[i];
      }
    }
  }
}

