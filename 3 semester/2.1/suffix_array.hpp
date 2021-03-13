#ifndef SUFFIX_ARRAY_H
#define SUFFIX_ARRAY_H


#include <string>
#include <vector>


/// Build suffix array in O(n*log(n)).
std::vector<int> BuildSuffixArray(const std::string& str);

class SuffixArray {

  public:

    SuffixArray(const std::string& str);

    std::vector<int> GetData() { return sorted_; };

  private:

    /// ` symbol precedes 'a', so it is less than any symbol in the alphabet.
    static const char   least_ch_ = '`';
    static const size_t alphabet_power_ = 27; // Include `.
    /// Equivalence classes of previous step.
    std::vector<int> eq_classes_;
    /// Array of sorted suffixes indexes on perious (k - 1) step.
    std::vector<int> sorted_;
    /// Used in CountSort as a count array. It is class member to avoid
    /// unwanted copies and reallocation.
    std::vector<int> count_;
    /// Intermediate array. Used to avoid a lot of vectors copies.
    std::vector<int> tmp_;

    void InitEqClasses(const std::string& str);
    void FillEqClasses(size_t length);
    void CountSort();
    void SortSecondComponents(size_t length);

    inline size_t GetIndex(char ch) { return ch - least_ch_; }
};


#endif // SUFFIX_ARRAY_H

