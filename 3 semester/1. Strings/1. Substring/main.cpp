#include <iostream>
#include <vector>

std::vector<int> BuildPrefixFunction(const std::string& s) {
  std::vector<int> pi(s.size(), 0);
  int compare_pos = 0;

  for (int i = 1; i < s.size(); ++i) {
    compare_pos = pi[i - 1];
    while (s[compare_pos] != s[i] && compare_pos != 0) {
      compare_pos = pi[compare_pos - 1];
    }
    if (s[compare_pos] == s[i]) {
      pi[i] = ++compare_pos;
    }
    // If s[compare_pos] != s[i] then compare_pos is 0.
  }
  return pi;
}

bool FindSubstringOccurrence(const std::string& sample, std::vector<int>& pi,
                             int& compare_pos, char symbol) {
  while (sample[compare_pos] != symbol && compare_pos != 0) {
    compare_pos = pi[compare_pos - 1];
  }
  if (sample[compare_pos] == symbol) {
    ++compare_pos;
  }
  if (compare_pos == sample.size()) {
    // This case `compare_pos` is out of sample size so next `while` statement
    // and lower line will be passed anyway.
    compare_pos = pi[compare_pos - 1];
    return true;
  }
  return false;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);

  std::string sample;
  std::cin >> sample;

  std::vector<int> pi = BuildPrefixFunction(sample);

  char text_symbol;
  int prev_pi_value = 0;
  for (int i = 0; std::cin >> text_symbol; ++i) {
    if (FindSubstringOccurrence(sample, pi, prev_pi_value, text_symbol)) {
      std::cout << i - (sample.size() - 1) << ' ';
    }
  }

  return 0;
}
