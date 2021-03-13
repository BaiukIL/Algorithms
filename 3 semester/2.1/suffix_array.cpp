#include "suffix_array.hpp"


/// Build suffix array in O(n*log(n)).
std::vector<int> BuildSuffixArray(const std::string& str) {

  std::vector<int> suf = SuffixArray(str).GetData();
  // Delete extra 'helper' symbol (`) result.
  suf.erase(suf.begin());
  return suf;
}

SuffixArray::SuffixArray(const std::string& str)
    : eq_classes_(str.size() + 1, 0),
      sorted_    (str.size() + 1, 0),
      count_     (str.size() + 1, 0),
      tmp_       (str.size() + 1, 0) {

  if (str.size() + 1 < alphabet_power_) {
    count_.resize(alphabet_power_);
  }

  InitEqClasses(str);
  CountSort();

  for (size_t length = 2; length < str.size(); length *= 2) {
    SortSecondComponents(length);
    CountSort();
    FillEqClasses(length);
  }
}

void SuffixArray::InitEqClasses(const std::string& str) {

  for (size_t i = 0; i < sorted_.size(); ++i) {
    tmp_[i] = i;
    if (i != sorted_.size() - 1) {
      eq_classes_[i] = GetIndex(str[i]);
    } else {
      eq_classes_[i] = GetIndex(least_ch_);
    }
  }
}

void SuffixArray::FillEqClasses(size_t length) {

  size_t classes_count = 1;
  // tmp is a new classes of equivalene.
  tmp_[sorted_[0]] = 0;

  for (size_t i = 1; i < eq_classes_.size(); ++i) {

    size_t prev_first_part_pos = sorted_[i - 1];
    size_t prev_second_part_pos =
        (prev_first_part_pos + length / 2) % eq_classes_.size();

    size_t cur_first_part_pos = sorted_[i];
    size_t cur_second_part_pos =
        (cur_first_part_pos  + length / 2) % eq_classes_.size();

    if (eq_classes_[prev_first_part_pos]  != eq_classes_[cur_first_part_pos] ||
        eq_classes_[prev_second_part_pos] != eq_classes_[cur_second_part_pos]) {

      ++classes_count;
    }
    tmp_[sorted_[i]] = classes_count - 1;
  }
  eq_classes_ = tmp_;
}

/// Count-sort by classes equivalence.
void SuffixArray::CountSort() {

  // Zero count array.
  count_.assign(count_.size(), 0);

  for (size_t eq : eq_classes_) {
    ++count_[eq];
  }
  // `count[eq] = c` means that block of symbols within `eq` class equivalence
  // ends in `c - 1` position.
  for (size_t eq = 1; eq < eq_classes_.size(); ++eq) {
    count_[eq] += count_[eq - 1];
  }
  // Start bypass from the end.
  // tmp is an array of substrings, sorted by second component.
  for (int i = tmp_.size() - 1; i >= 0; --i) {
    sorted_[--count_[eq_classes_[tmp_[i]]]] = tmp_[i];
  }
}

void SuffixArray::SortSecondComponents(size_t length) {
  // Here we need to sort pairs by second component, but we have already sorted
  // ones on previous step. The only thing we should do is to decrease indexes
  // of `sorted_` by length/2 to get corresponding beginnings.
  // Thus we get array of first components, sorted by second one.
  // For more detailed explanation see https://e-maxx.ru/algo/suffix_array
  for (size_t i = 0; i < sorted_.size(); ++i) {
    tmp_[i] = sorted_[i] - (length / 2);
    if (tmp_[i] < 0) {
      tmp_[i] += sorted_.size();
    }
  }
}

