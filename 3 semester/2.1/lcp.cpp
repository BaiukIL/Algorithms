#include "lcp.hpp"


std::vector<int> GetBackSuffixArray(const std::vector<int>& suf) {
  std::vector<int> back_suf(suf.size());

  for (size_t i = 0; i < suf.size(); ++i) {
    back_suf[suf[i]] = i;
  }

  return back_suf;
}

/// Kasai algorithm.
std::vector<int> GetLcp(const std::string& str) {

  std::vector<int> suf = BuildSuffixArray(str);
  std::vector<int> back_suf = GetBackSuffixArray(suf);
  std::vector<int> lcp(suf.size(), 0);

  size_t ancestor_lcp = 0;

  for (size_t i = 0; i < suf.size(); ++i) {
    size_t compare_pos = 0;

    if (ancestor_lcp > 1) {
      compare_pos += ancestor_lcp - 1;
    }

    // The last suffix in suf array.
    if (back_suf[i] == suf.size() - 1) {
      lcp[i] = ancestor_lcp = 0;
      continue;
    }

    while (str[i + compare_pos] == str[suf[back_suf[i] + 1] + compare_pos] &&
           i + compare_pos < suf.size()                                    &&
           suf[back_suf[i] + 1] + compare_pos < suf.size()) {

      ++compare_pos;
    }

    lcp[i] = ancestor_lcp = compare_pos;
  }

  return lcp;
}

