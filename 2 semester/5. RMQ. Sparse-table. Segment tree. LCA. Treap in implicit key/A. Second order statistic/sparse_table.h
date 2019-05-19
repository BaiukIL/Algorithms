#ifndef UNTITLED_SPARSE_TABLE_H
#define UNTITLED_SPARSE_TABLE_H

#include <iostream>
#include <vector>
#include <unordered_set>
#include <cmath>

class SparseTable {
 public:
  explicit SparseTable(const std::vector<int> &array);

  int GetMin(uint left, uint right) const;

 private:
  std::vector<std::vector<std::pair<int, int>>> table;
  std::vector<int> array;
  std::vector<int> logs;

  void FillTable();
  void FillLogs();
  // This function finds and returns the 1st and the 2nd order statistic indexes (of given pairs)
  std::pair<int, int> GetOrderStatIndexes(const std::pair<int, int> &indexes1, const std::pair<int, int> &indexes2) const;
};


#endif //UNTITLED_SPARSE_TABLE_H
