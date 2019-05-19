#include "SegmentTree.h"

void ExtendArray(std::vector<int> &array) {
  //  add array size to the closest power of 2
  int sizeInPowerOf2 = 1;
  while (sizeInPowerOf2 < array.size()) {
    sizeInPowerOf2 *= 2;
  }
  int difference = sizeInPowerOf2 - array.size();
  for (int i = 0; i < difference; ++i) {
//    0 is neutral element in task context
    array.push_back(0);
  }
}

void SegmentTree::FillLogs() {
  for (int i = 1; i <= heapSize; ++i) {
    logs[i] = static_cast<int>(std::log2(i));
  }
}

void SegmentTree::FillLowLevel(const std::vector<int> &array) {
  for (int index = 0; index < array.size(); ++index) {
    int treeIndex = array.size() + index - 1;
    max[treeIndex] = suffixes[treeIndex] = prefixes[treeIndex] = array[index];
  }
}

void SegmentTree::SetPrefix(int index) {
  int leftChild = 2*index + 1, rightChild = 2*index + 2;
  if (prefixes[leftChild] == MaxLevelNum(leftChild)) {
    prefixes[index] = prefixes[leftChild] + prefixes[rightChild];
  } else {
    prefixes[index] = prefixes[leftChild];
  }
}

void SegmentTree::SetSuffix(int index) {
  int leftChild = 2*index + 1, rightChild = 2*index + 2;
  if (suffixes[rightChild] == MaxLevelNum(leftChild)) {
    suffixes[index] = suffixes[leftChild] + suffixes[rightChild];
  } else {
    suffixes[index] = suffixes[rightChild];
  }
}

void SegmentTree::FillUpperLevels(const std::vector<int> &array) {
  for (int i = array.size() - 2; i >= 0; --i) {
    SetPrefix(i);
    SetSuffix(i);
    int leftChild = 2*i + 1, rightChild = 2*i + 2;
    max[i] = std::max({prefixes[i], suffixes[i],
                       max[leftChild], max[rightChild],
                       suffixes[leftChild] + prefixes[rightChild]});
  }
}

int SegmentTree::MaxLevelNum(int index) const {
  int height = logs[heapSize] - logs[index + 1];
  return (1 << height);
}

void SegmentTree::HandleLeftBorder(int &result, int left, int &prefix) const {
  if (prefixes[left] == MaxLevelNum(left)) {
    prefix += prefixes[left];
  } else {
    result = std::max({prefix + prefixes[left], max[left], result});
    prefix = suffixes[left];
  }
}

void SegmentTree::HandleRightBorder(int &result, int right, int &suffix) const {
  if (suffixes[right] == MaxLevelNum(right)) {
    suffix += suffixes[right];
  } else {
    result = std::max({suffix + suffixes[right], max[right], result});
    suffix = prefixes[right];
  }
}

// not const & because it's comfortable to change array
SegmentTree::SegmentTree(std::vector<int> array) : heapSize(0), max(0), prefixes(0), suffixes(0), logs(0) {
//  add array size to the closest power of 2
  ExtendArray(array);
  heapSize = 2*array.size() - 1;
  max.resize(heapSize);
  prefixes.resize(heapSize);
  suffixes.resize(heapSize);
  logs.resize(heapSize);
  FillLogs();
  FillLowLevel(array);
  FillUpperLevels(array);
}

int SegmentTree::GetMaxUnitsNumber(int left, int right) const {
  int result = 0;
  int prefix = 0, suffix = 0;
  left += (heapSize - 1) / 2;
  right += (heapSize - 1) / 2;

  while (left <= right) {
//    if left border is right child of its parent
    if (left % 2 == 0) {
      HandleLeftBorder(result, left, prefix);
      ++left;
    }
//    if right border is left child of its parent
    if (right % 2 == 1) {
      HandleRightBorder(result, right, suffix);
      --right;
    }

    left /= 2;
    right = (right - 2) / 2;
    if (left + 1 == right) {
      result = std::max(suffixes[left] + prefixes[right], result);
    }
    result = std::max({result, suffix, prefix});
  }
  return result;
}