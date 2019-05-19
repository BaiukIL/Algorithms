#include "sparse_table.h"

SparseTable::SparseTable(const std::vector<int> &array) : table(1), logs(0) {
//    copy array
  this->array = array;
  table[0].clear();
//  fill initial level
  for (int i = 0; i < array.size(); ++i) {
    table[0].push_back(std::make_pair(i, i));
  }
  FillTable();
  FillLogs();
}

void SparseTable::FillTable() {
  uint level = 1;
//    size of carriage on current level is 2 in power of (level - 1)
  uint window = 1;
  while(window < table[level - 1].size()) {
//      since 1st level store indexes to keep elements uniqueness
    table.emplace_back(std::vector<std::pair<int, int>>(table[level - 1].size() - window));
    for (uint i = 0; i < table[level - 1].size() - window; ++i) {
      table[level][i] = GetOrderStatIndexes(table[level - 1][i], table[level - 1][i + window]);
    }
    ++level;
    window <<= 1;
  }
}

void SparseTable::FillLogs() {
  logs.resize(table[0].size() + 1);
  for (uint i = 1; i <= table[0].size(); ++i) {
    logs[i] = static_cast<int>(std::log2(i));
  }
}

int SparseTable::GetMin(uint left, uint right) const  {
  uint level = logs[right - left];
  uint secondOrderStatIndex = GetOrderStatIndexes(table[level][left], table[level][right - (1 << level) + 1]).second;
  return array[secondOrderStatIndex];
}

std::pair<int, int> SparseTable::GetOrderStatIndexes(const std::pair<int, int> &indexes1,
                                                     const std::pair<int, int> &indexes2) const {
//  use unordered_set to avoid index duplicates
  std::unordered_set<int> indexes;
  indexes.insert(indexes1.first);
  indexes.insert(indexes1.second);
  indexes.insert(indexes2.first);
  indexes.insert(indexes2.second);
  //  1st and 2nd order statistics respectively
  std::pair<int, int> orderStatIndex = std::make_pair(-1, -1);
  for (int index: indexes) {
    if (orderStatIndex.first == -1 || array[index] < array[orderStatIndex.first]) {
      orderStatIndex.second = orderStatIndex.first;
      orderStatIndex.first = index;
    } else if (orderStatIndex.second == -1 || array[index] < array[orderStatIndex.second]) {
      orderStatIndex.second = index;
    }
  }
  return orderStatIndex;
}