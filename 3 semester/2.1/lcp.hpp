#ifndef LCP_H
#define LCP_H


#include <string>
#include <vector>
#include "suffix_array.hpp"

std::vector<int> GetBackSuffixArray(const std::vector<int>& suf);

/// Kasai algorithm.
std::vector<int> GetLcp(const std::string& str);

#endif // LCP_H

