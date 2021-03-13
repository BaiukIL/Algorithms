#include <iostream>
#include <string>
#include "lcp.hpp"

size_t CountSubstrings(const std::string& str) {
  size_t count = 0;
  size_t size = str.size();

  std::vector<int> lcp = GetLcp(str);

  for (size_t i = 0; i < size; ++i) {
    count += size - i - lcp[i];
  }

  return count;
}

int main() {

  std::string pattern;
  std::cin >> pattern;

  std::cout << "Suf: ";
  auto v = BuildSuffixArray(pattern);
  for (int i : v){
    std::cout << i << ' ';
  }
  std::cout << '\n' << "lcp: ";
  for (int i : GetLcp(pattern)) {
    std::cout << i << ' ';
  }
  std::cout << "Sub: " << CountSubstrings(pattern);

  return 0;
}
