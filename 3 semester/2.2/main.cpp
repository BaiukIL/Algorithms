#include <iostream>
#include <string>
#include "suffix_tree.hpp"


int main() {
  std::string string1;
  std::string string2;

  std::cin >> string1 >> string2;

  std::string string = string1 + string2;
  SuffixTree tree(string);

  std::cout << tree.size() << '\n';
  // `parent[i] == j` means that `i` is a son of `j` node.
  std::vector<size_t> parents(tree.size(), 0);
  // Alias for comfortable nodes access.
  auto& nodes = tree.data();
  // Number of current node in pre-order lexicographical bypass.
  size_t current = 0;
  // Use pre-order bypass.
  for (auto iter = tree.begin_pre(); !iter.end(); ++iter) {
    if (*iter == 0) { continue; } // Skip root.
    ++current;

    for (int i = SuffixTree::alphabet_power - 1; i >= 0; --i) {
      if (nodes[*iter].sons_[i] != 0) {
        parents[nodes[*iter].sons_[i]] = current;
      }
    }

    size_t left = nodes[*iter].left_;
    size_t right = nodes[*iter].IsLeaf() ? string.size() :
                                           nodes[*iter].right_ + 1;
    size_t string_num = 0;
    // First string.
    if (left < string1.size()) {
      if (right > string1.size()) {
        right = string1.size();
      }
      string_num = 0;
    // Second string.
    } else {
      left -= string1.size();
      right -= string1.size();
      string_num = 1;
    }
    printf("%lu %lu %lu %lu\n", parents[*iter],
                                string_num,
                                left,
                                right);
  }

  return 0;
}

