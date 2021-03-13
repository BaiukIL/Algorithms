#include "disjoint_set.h"

element_type DisjointSet::Find(element_type element) {
  return parent[element] == element ? parent[element] : parent[element] = Find(parent[element]);
}

void DisjointSet::Union(element_type first, element_type second) {
  element_type firstParent = Find(first);
  element_type secondParent = Find(second);
  if (firstParent == secondParent) {
    return;
  }
  if (rank[firstParent] < rank[secondParent]) {
    parent[firstParent] = secondParent;
  } else {
    parent[secondParent] = firstParent;
    if (rank[firstParent] == rank[secondParent]) {
      ++rank[firstParent];
    }
  }
}

void DisjointSet::MakeSet(element_type element) {
  parent[element] = element;
  rank[element] = 0;
}
