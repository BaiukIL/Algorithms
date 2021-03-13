#ifndef SUFFIX_TREE_HPP
#define SUFFIX_TREE_HPP

#include <vector>
#include <string>
#include <stack>


inline size_t GetCharId(char ch);


class SuffixTree {

  public:

    class Node;
    class DFSIterator;
    class PreOrderIterator;
    class PostOrderIterator;

    typedef Node        value_type;
    typedef DFSIterator iterator;
    // Alphabet power is 28: 'a, b, ..., z, $ and # (trailing chars)'
    static const size_t alphabet_power = 28;

  private:

    /// Captured string.
    std::string string_;
    /// Nodes array.
    std::vector<Node> nodes_;
    /// Current number of nodes in the tree.
    size_t nodes_num_;
    /// Length of accumulated substring.
    size_t remainder_;
    /// Active node index (in nodes array).
    size_t active_node_;
    /// Length of active edge.
    size_t active_length_;
    /// Active edge identifier. string_[active_edge_] == active_char
    size_t active_edge_;
    /// Position of last read string character.
    size_t read_pos_;
    /// Pointer to unset suffix link.
    size_t* unset_suf_;

  public:

    SuffixTree();
    SuffixTree(const std::string& string);

    void AddString(const std::string& string);

    size_t size() const;
    std::vector<Node>& data();
    /// Return pre-order iterator.
    PreOrderIterator  begin_pre();
    /// Return post-order iterator.
    PostOrderIterator begin_post();

  private:

    /// Return string position of last passed character.
    /// i.e. if string = abcabc and edge = ab|cab, then GetActiveCharPos == 1.
    size_t GetActiveCharPos();
    /// Return created inner node.
    size_t SplitEdge();
    /// Create leaf node and hang it from `node`.
    void HangLeaf();
    /// If have unset suffix link, assign it with `node`.
    void UpdateSuffixLink(size_t node);
    bool CanGoByReadChar();
    /// Go by active edge.
    void GoByChar();
    /// Walk down the nodes until active length less than active edge length.
    void UpdateActiveNode();
    /// Go by active node suffix link.
    void GoBySuffixLink();
    /// Return active edge length.
    size_t GetEdgeLength();
};

struct SuffixTree::Node {
  /// string[left] is the most left string character of edge to parent.
  int left_;
  /// string[right] is the most right one.  size_t right_;
  int right_;
  /// Suffix node index in nodes array.
  size_t suffix_link_;
  /// `sons_[i] == j` means that node has edge by character with `i` index
  /// (in the alphabet) and this character has `j` position in the string.
  size_t sons_[SuffixTree::alphabet_power];

  Node(int left, int right) : left_(left),
                              right_(right),
                              suffix_link_(0),
                              sons_() {}

  size_t GetSon(char ch);
  bool HasEdge(char ch);
  void AddSon(size_t node, char ch);
  bool IsLeaf();
};


class SuffixTree::DFSIterator {

  public:
    // Necessary typedefs.
    typedef SuffixTree::value_type    value_type;
    typedef value_type&               reference;
    typedef value_type*               pointer;
    typedef size_t                    difference_type;
    typedef std::forward_iterator_tag iterator_category;

  protected:

    std::vector<value_type>& nodes_;
    // Stack stores indexes.
    std::stack<size_t> dfs_;

  public:

    DFSIterator(std::vector<value_type>& nodes);

    DFSIterator& operator++();
    /// Return array index of current node.
    size_t operator*();
    // Return true if iterator is over.
    bool end();
};

class SuffixTree::PreOrderIterator : public SuffixTree::DFSIterator {

  public:

    PreOrderIterator(std::vector<value_type>& nodes);
    PreOrderIterator& operator++();
};


class SuffixTree::PostOrderIterator : public SuffixTree::DFSIterator {

  protected:

    // `is_visited[node] = true` if `node` has been visited before.
    std::vector<char> is_visited_;

  public:

    PostOrderIterator(std::vector<value_type>& nodes);
    PostOrderIterator& operator++();
};


#endif // SUFFIX_TREE_HPP

