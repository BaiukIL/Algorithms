#include "suffix_tree.hpp"


size_t GetCharId(char ch) {
  size_t id = 0;
  if ('a' <= ch && ch <= 'z') {
    id = ch - 'a' + 2;
  } else {
    if (ch == '$') {
      id = 1;
    } else {
      id = 0;
    }
  }
  return id;
}

//================================= TREE ======================================

SuffixTree::SuffixTree()
    : string_(),
      nodes_(1, Node(0, 0)),  // Root.
      nodes_num_(1),
      remainder_(0),
      active_node_(0),
      active_length_(0),
      active_edge_(0),
      read_pos_(0),
      unset_suf_(nullptr) {}

SuffixTree::SuffixTree(const std::string& string)
    : SuffixTree() {
  AddString(string);
}

void SuffixTree::AddString(const std::string& string) {
  string_ += string;
  // Suffix tree size < 2 * string.size
  nodes_.resize(string_.size() * 2, Node(-1, -1));

  while (read_pos_ < string_.size()) {

    ++remainder_;
    while (remainder_ > 0) {
      // If node does not have active edge, assign it with read character.
      if (active_length_ == 0) {
        active_edge_ = read_pos_;
      }

      if (CanGoByReadChar()) {
        UpdateSuffixLink(active_node_);
        GoByChar();
        break;
      } else {
        if (active_length_ == 0) { // If stopped in a node.
          HangLeaf();
          UpdateSuffixLink(active_node_);
        } else {                   // If stopped in an edge.
          size_t inner = SplitEdge();
          UpdateSuffixLink(inner);
          // Inner node has uset suffix link, so extract it for further setting.
          unset_suf_ = &nodes_[inner].suffix_link_;
        }
        --remainder_;
        GoBySuffixLink();
        UpdateActiveNode();
      }
    }
    ++read_pos_;
  }
}

size_t SuffixTree::size() const {
  return nodes_num_;
}

SuffixTree::PreOrderIterator SuffixTree::begin_pre() {
  return PreOrderIterator(nodes_);
}

SuffixTree::PostOrderIterator SuffixTree::begin_post() {
  return PostOrderIterator(nodes_);
}

std::vector<SuffixTree::value_type>& SuffixTree::data() {
  return nodes_;
}

size_t SuffixTree::GetActiveCharPos() {
  return nodes_[nodes_[active_node_].GetSon(string_[active_edge_])].left_
    + active_length_ - 1;
}

bool SuffixTree::CanGoByReadChar() {
  return active_length_ == 0 ?
           nodes_[active_node_].HasEdge(string_[read_pos_]) :
           string_[GetActiveCharPos() + 1] == string_[read_pos_];
}

void SuffixTree::GoByChar() {
  ++active_length_;
  if (active_length_ != 0) {
    UpdateActiveNode();
  }
}

void SuffixTree::UpdateActiveNode() {
  // Move active_node_ if active length is greater than edge length.
  size_t edge_length = GetEdgeLength();
  while (active_length_ != 0 && edge_length <= active_length_) {
    active_node_ = nodes_[active_node_].GetSon(string_[active_edge_]);
    active_edge_ += edge_length;
    active_length_ -= edge_length;

    edge_length = GetEdgeLength();
  }
}

size_t SuffixTree::GetEdgeLength() {
  size_t son = nodes_[active_node_].GetSon(string_[active_edge_]);
  size_t edge_length = 0;
  // Count edge length.
  if (nodes_[son].IsLeaf())
    edge_length = read_pos_ - nodes_[son].left_ + 1;
  else
    edge_length = nodes_[son].right_ - nodes_[son].left_ + 1;
  return edge_length;
}

void SuffixTree::HangLeaf() {
  size_t leaf = nodes_num_++;
  nodes_[leaf] = Node(read_pos_, -1);
  nodes_[active_node_].AddSon(leaf, string_[read_pos_]);
}

size_t SuffixTree::SplitEdge() {
  // Create nodes.
  size_t old_leaf = nodes_[active_node_].GetSon(string_[active_edge_]);
  size_t inner = nodes_num_++;
  size_t new_leaf = nodes_num_++;
  size_t split_pos = nodes_[old_leaf].left_ + active_length_ - 1;
  nodes_[inner] = Node(nodes_[old_leaf].left_, split_pos);
  nodes_[new_leaf] = Node(read_pos_, -1);
  nodes_[old_leaf].left_ = split_pos + 1;
  // Set sons.
  nodes_[active_node_].AddSon(inner, string_[nodes_[inner].left_]);
  nodes_[inner].AddSon(old_leaf, string_[nodes_[old_leaf].left_]);
  nodes_[inner].AddSon(new_leaf, string_[nodes_[new_leaf].left_]);
  return inner;
}

void SuffixTree::GoBySuffixLink() {
  if (active_node_ == 0 && active_length_ > 0) {
    // Shift active character right.
    active_edge_ = read_pos_ - remainder_ + 1;
    --active_length_;
  } else {
    active_node_ = nodes_[active_node_].suffix_link_;
  }
}

void SuffixTree::UpdateSuffixLink(size_t node) {
  if (unset_suf_ != nullptr) {
    *unset_suf_ = node;
    unset_suf_ = nullptr;
  }
}

//================================== NODE ======================================

size_t SuffixTree::Node::GetSon(char ch) {
  return sons_[GetCharId(ch)];
}

bool SuffixTree::Node::HasEdge(char ch) {
  return GetSon(ch) != 0;
}

void SuffixTree::Node::AddSon(size_t node, char ch) {
  sons_[GetCharId(ch)] = node;
}

bool SuffixTree::Node::IsLeaf() {
  return right_ == -1;
}

//================================ ITERATORS ===================================

SuffixTree::DFSIterator::DFSIterator(std::vector<value_type>& nodes)
    : nodes_(nodes),
      dfs_() {
  dfs_.push(0);
}

size_t SuffixTree::DFSIterator::operator*() {
  return dfs_.top();
}

bool SuffixTree::DFSIterator::end() {
  return dfs_.empty();
}

//------------------------------ PRE-ORDER -------------------------------------

SuffixTree::PreOrderIterator::PreOrderIterator(std::vector<value_type>& nodes)
    : SuffixTree::DFSIterator(nodes) {}

SuffixTree::PreOrderIterator&
SuffixTree::PreOrderIterator::operator++() {
  size_t current = dfs_.top();
  dfs_.pop();
  for (int i = SuffixTree::alphabet_power - 1; i >= 0; --i) {
    if (nodes_[current].sons_[i] != 0) {
      dfs_.push(nodes_[current].sons_[i]);
    }
  }
  return *this;
}

//------------------------------ POST-ORDER ------------------------------------

SuffixTree::PostOrderIterator::PostOrderIterator(std::vector<value_type>& nodes)
    : SuffixTree::DFSIterator(nodes),
      is_visited_(nodes.size(), false) {}

SuffixTree::PostOrderIterator&
SuffixTree::PostOrderIterator::operator++() {
  dfs_.pop();
  size_t current = dfs_.empty() ? 0 : dfs_.top(); // Bodge.
  while (!nodes_[current].IsLeaf() && !is_visited_[current]) {
    for (int i = SuffixTree::alphabet_power - 1; i >= 0; --i) {
      if (nodes_[current].sons_[i] != 0) {
        dfs_.push(nodes_[current].sons_[i]);
      }
    }
    is_visited_[current] = true;
    current = dfs_.top();
  }
  return *this;
}

