#ifndef TRIE_H
#define TRIE_H

#include <vector>
#include <string_view>

class Trie {
  // Typedefs.
  public:

    typedef int index_t;

  // Fields.
  private:

    struct Node;

    /// Array of nodes.
    std::vector<Node> nodes_;

    /// Index of current node.
    index_t cur_node_id_;

    /// Number of alphabet characters.
    static const int alphabet_power = 26;

    /// Root index is 0.
    /// It is same for every trie, so it is static const.
    static const index_t root_id = 0;

  public:

    Trie() : nodes_(1, Node(root_id, root_id)),
             cur_node_id_(root_id) {}

    ~Trie() = default;

    /// Add sample to trie.
    template <typename T>
    void AddSample(const T& sample, int sample_id);

    /// Go by character in trie.
    void ChangeState(char ch);

    friend void FillResult(Trie& trie, int pos, std::vector<int>& result,
                           const std::vector<std::string_view>& samples,
                           const std::vector<int>& samples_mask_pos);

  private:

    struct Node {
      Node(index_t parent, char parent_ch)
          : sons_       (alphabet_power, -1),
            go_         (alphabet_power, -1),
            parent_     (parent),
            parent_ch_  (parent_ch),
            suffix_     (-1),
            term_suffix_(-1),
            is_terminal_(false),
            patterns_id_() {}

      /**
       * @brief Array of son nodes indexes.
       *
       * sons_[i] is an index of node that can be reached right from current
       * by character with `i` index in the alphabet.
       */
      std::vector<index_t> sons_;

      /**
       * @brief Array of next nodes.
       *
       * go_[i] is an index of node that is reached from current after all
       * suffix/sons transitions by character with `i` index in the
       * alphabet are done.
       */
      std::vector<index_t> go_;

      /// Parent index.
      index_t parent_;

      /// Alphabet index of the ch that leads from parent to current node.
      char parent_ch_;

      /// Suffix node index.
      index_t suffix_;

      /// Terminal suffix node index.
      index_t term_suffix_;

      /// True if node is terminal (i.e. if pattern ends here)
      bool is_terminal_;

      /// Indexes of patterns which ends in this node.
      std::vector<index_t> patterns_id_;
    };

    /// Return symbol index in the alphabet.
    inline int GetSymbolId(char ch) { return ch - 'a'; }

    index_t GetSuffix(index_t node_id);
    index_t GetTermSuffix(index_t node_id);
    index_t GetGo(index_t node_id, char ch);
};

// Template stuff is to be placed in header.
template <typename T>
void Trie::AddSample(const T& sample, int sample_id) {
  index_t cur_id = root_id;

  for (char ch : sample) {
    ch = GetSymbolId(ch);

    if (nodes_[cur_id].sons_[ch]  == -1) {
      nodes_.emplace_back(Node(cur_id, ch));
      nodes_[cur_id].sons_[ch] = nodes_.size() - 1;
    }
    cur_id = nodes_[cur_id].sons_[ch];
  }

  nodes_[cur_id].is_terminal_ = true;
  nodes_[cur_id].patterns_id_.push_back(sample_id);
}

#endif // TRIE_H
