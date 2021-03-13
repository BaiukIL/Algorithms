#include <iostream>
#include <string>
#include "suffix_tree.hpp"


class SubstringFinder {

  private:

    SuffixTree tree_;
    // Full string - concatenation of first and second strings.
    std::string string_;
    std::string string1_;
    std::string string2_;

    SuffixTree::value_type* nodes_;
    // Array of nodes depths.
    std::vector<size_t> depth_;
    // `is_common_node_[node] == true` means node parent edge is a substring
    // of both strings.
    std::vector<char> is_common_node_;
    /// Result string position in lexical order.
    size_t res_lexic_order_pos_;

  public:

    SubstringFinder(const std::string string1,
                    const std::string string2,
                    size_t res_lexic_order_pos);
    std::string FindSubstring();

  private:
    /// Fill is_common_node_ which shows if node has parent edge which is in
    /// both strings.
    void FindCommonNodes();
    void FillDepth();
};

SubstringFinder::SubstringFinder(const std::string string1,
                                 const std::string string2,
                                 size_t res_lexic_order_pos)

    : tree_(),
      string_(),
      string1_(string1 + "#"),
      string2_(string2 + "$"),
      nodes_(),
      depth_(),
      res_lexic_order_pos_(res_lexic_order_pos),
      is_common_node_() {

  tree_ = SuffixTree(string_);
  string_ = string1_ + string2_;
  nodes_ = tree_.data().data();
  depth_.resize(tree_.size(), 0);
  is_common_node_.resize(tree_.size(), false);

  FillDepth();
  FindCommonNodes();
}

std::string SubstringFinder::FindSubstring() {

  size_t cur_lexic_order_pos = 0;
  std::string result = "";
  // increment iterator to skip the root.
  for (auto iter = ++tree_.begin_pre(); !iter.end(); ++iter) {
    if (is_common_node_[*iter]) {
      // Add edge length and count different substrings therefore.
      cur_lexic_order_pos += nodes_[*iter].right_ - nodes_[*iter].left_ + 1;
      if (cur_lexic_order_pos >= res_lexic_order_pos_) {
        size_t remainder = cur_lexic_order_pos - res_lexic_order_pos_;
        result = string_.substr(
            nodes_[*iter].right_ + 1 - depth_[*iter],
            depth_[*iter] - remainder);
        break;
      }
    }
  }
  return result;
}

void SubstringFinder::FindCommonNodes() {

  for (auto iter = tree_.begin_post(); !iter.end(); ++iter) {
    if (nodes_[*iter].IsLeaf()) { continue; }

    bool has_edge_from_str1 = false;
    bool has_edge_from_str2 = false;

    for (auto son : nodes_[*iter].sons_) {
      // If son does not exist skip a step.
      if (son == 0) { continue; }
      // If node has edge which starts in string1_.
      if (nodes_[son].left_ < string1_.size()) {
        has_edge_from_str1 = true;
      // If node has edge which starts in string2_.
      } else {
        has_edge_from_str2 = true;
      }
      // Or if son itself is a both strings substring.
      if (is_common_node_[son]) {
        has_edge_from_str1 = true;
        has_edge_from_str2 = true;
      }
    }
    if (has_edge_from_str1 && has_edge_from_str2) {
      is_common_node_[*iter] = true;
    }
  }
}

void SubstringFinder::FillDepth() {

  for (auto iter = tree_.begin_pre(); !iter.end(); ++iter) {
    // Skip leaves to optimize.
    if (nodes_[*iter].IsLeaf()) { continue; }
    for (auto son : nodes_[*iter].sons_) {
      // If son exists.
      if (son != 0) {
        // Son depth = current_depth + edge_length.
        depth_[son] = depth_[*iter] + nodes_[son].right_ - nodes_[son].left_ + 1;
      }
    }
  }
}

int main() {
  std::string string1;
  std::string string2;
  // Desired position.
  size_t res_lexic_order_pos = 0;

  std::cin >> string1 >> string2 >> res_lexic_order_pos;

  SubstringFinder solver(string1, string2, res_lexic_order_pos);

  std::string result = solver.FindSubstring();
  if (result.empty()) {
    std::cout << -1;
  } else {
    std::cout << result;
  }

  return 0;
}

