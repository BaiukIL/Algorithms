#ifndef UNTITLED_SEGMENTTREE_H
#define UNTITLED_SEGMENTTREE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

class SegmentTree {
 public:
  explicit SegmentTree(std::vector<int> array);

  int GetMaxUnitsNumber(int left, int right) const;
 private:
  int heapSize;
  std::vector<int> max;
  std::vector<int> prefixes;
  std::vector<int> suffixes;
//  I haven't managed to find out std::log2 difficulty, so count it in
//  preprocessing to be sure it doesn't affect asymptotic
  std::vector<int> logs;

  void FillLogs();
  void FillLowLevel(const std::vector<int> &array);
  void FillUpperLevels(const std::vector<int> &array);
  int MaxLevelNum(int index) const;
  void SetPrefix(int index);
  void SetSuffix(int index);
  void HandleLeftBorder(int &result, int left, int &prefix) const;
  void HandleRightBorder(int &result, int right, int &suffix) const;
};

#endif //UNTITLED_SEGMENTTREE_H
