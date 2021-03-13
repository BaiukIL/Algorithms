#include "trie.h"

Trie::index_t Trie::GetSuffix(index_t node_id) {
  Node& node = nodes_[node_id];

  if (node.suffix_ == -1) {
    if (node_id == root_id || node.parent_ == root_id) {
      node.suffix_ = root_id;
    } else {
      node.suffix_ = GetGo(GetSuffix(node.parent_), node.parent_ch_);
    }
  }
  return node.suffix_;
}

Trie::index_t Trie::GetGo(index_t node_id, char ch) {
  Node& node = nodes_[node_id];

  if (node.go_[ch] == -1) {
    if (node.sons_[ch] != -1) {
      node.go_[ch] = node.sons_[ch];
    } else {
      if (node_id == root_id) {
        node.go_[ch] = root_id;
      } else {
        node.go_[ch] = GetGo(GetSuffix(node_id), ch);
      }
    }
  }
  return node.go_[ch];
}

Trie::index_t Trie::GetTermSuffix(index_t node_id) {
  Node& node = nodes_[node_id];
  if (node.term_suffix_ == -1) {
    if (nodes_[GetSuffix(node_id)].is_terminal_) {
      node.term_suffix_ = GetSuffix(node_id);
    } else {
      if (GetSuffix(node_id) == root_id) {
        node.term_suffix_ = root_id;
      } else {
        node.term_suffix_ = GetTermSuffix(GetSuffix(node_id));
      }
    }
  }
  return node.term_suffix_;
}

void Trie::ChangeState(char ch) {
  cur_node_id_ = GetGo(cur_node_id_, GetSymbolId(ch));
}
