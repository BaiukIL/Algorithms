#ifndef MY_CPP_PROJECT_DISJOINT_SET_H
#define MY_CPP_PROJECT_DISJOINT_SET_H

#include <iostream>
#include <vector>

typedef uint element_type;

class DisjointSet {
 public:
  explicit DisjointSet(size_t size) : parent(size), rank(size) {}
  element_type Find(element_type element);
  void Union(element_type first, element_type second);
  void MakeSet(element_type element);

 private:
  std::vector<element_type> parent;
//  this value almost never more than uint8_t
  std::vector<uint8_t> rank;
};

#endif //MY_CPP_PROJECT_DISJOINT_SET_H
