#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "trie.h"

inline void CountPatternBegin(int id, int pos,
                              int sample_size, int sample_mask_begin_pos,
                              std::vector<int>& result) {

  int mask_begin_index = (pos + 1 - sample_size) - sample_mask_begin_pos;

  if (mask_begin_index >= 0) {
    ++result[mask_begin_index];
  }
}

void FillResult(Trie& trie, int pos, std::vector<int>& result,
                const std::vector<std::string_view>& samples,
                const std::vector<int>& samples_mask_pos) {

  if (trie.nodes_[trie.cur_node_id_].is_terminal_) {
    for (auto& id : trie.nodes_[trie.cur_node_id_].patterns_id_) {
      CountPatternBegin(
          id, pos, samples[id].size(), samples_mask_pos[id], result);
    }
  }

  // Jump over terminal suffixes.
  Trie::index_t terminal_id = trie.cur_node_id_;
  while ((terminal_id = trie.GetTermSuffix(terminal_id)) != Trie::root_id) {
    for (auto& id : trie.nodes_[terminal_id].patterns_id_) {
      CountPatternBegin(
          id, pos, samples[id].size(), samples_mask_pos[id], result);
    }
  }
}

void ParseToSamples(const std::string& mask,
                    std::vector<std::string_view>& samples,
                    std::vector<int>& samples_mask_pos) {

  int str_begin = 0, str_end = 0;

  for (int i = 0; i < mask.size(); ) {
    while (mask[i] == '?') {
      ++i;
    }
    str_begin = i;

    while (mask[i] != '?' && i < mask.size()) {
      ++i;
    }
    str_end = i;

    if (str_end - str_begin > 0) {
      samples.emplace_back(
          std::string_view(mask.data() + str_begin, str_end - str_begin));
      samples_mask_pos.push_back(str_begin);
    }
  }
}

void FindSamplesEntriesPositions(std::vector<int>& result,
                                 const std::vector<std::string_view>& samples,
                                 const std::vector<int>& samples_mask_pos,
                                 const std::string& text,
                                 size_t last_queries_length) {

  Trie trie;

  for (int id = 0; id < samples.size(); ++id) {
    trie.AddSample(samples[id], id);
  }

  for (int pos = 0; pos < (int)text.size() - last_queries_length; ++pos) {
    trie.ChangeState(text[pos]);
    FillResult(trie, pos, result, samples, samples_mask_pos);
  }
}

size_t GetLastQueriesClasterSize(const std::string& mask) {
  size_t i = mask.size() - 1;
  while (mask[i] == '?') {
    --i;
  }

  return mask.size() - 1 - i;
}

void HandleFullQueriesMask(size_t mask_size, size_t text_size) {
  for (int pos = 0; pos < text_size - mask_size + 1; ++pos) {
    std::cout << pos << ' ';
  }
}

void OutputResult(const std::vector<int>& result, int samples_count) {
  for (int pos = 0; pos < result.size(); ++pos) {
    if (result[pos] == samples_count) {
      std::cout << pos << ' ';
    }
  }
}

int main() {
  // We also can read text online, but it does not
  // influence memory and time asymptotic.
  std::string mask, text;
  std::cin >> mask >> text;

  std::vector<std::string_view> samples;
  std::vector<int> samples_mask_pos;

  ParseToSamples(mask, samples, samples_mask_pos);

  std::vector<int> result(text.size(), 0);

  if (!samples.empty()) {
    FindSamplesEntriesPositions(result, samples, samples_mask_pos, text,
                                GetLastQueriesClasterSize(mask));
    OutputResult(result, samples.size());
  } else {
    HandleFullQueriesMask(mask.size(), text.size());
  }

  return 0;
}
