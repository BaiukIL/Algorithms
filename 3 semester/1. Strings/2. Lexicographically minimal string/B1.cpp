#include <iostream>
#include <unordered_set>
#include <vector>

void FillAlphabet(std::vector<char>& alphabet) {
  for (char ch = 'a', i = 0; ch <= 'z'; ++ch, ++i) {
    alphabet[i] = ch;
  }
}

void FindIllegalLetters(std::unordered_set<char>& illegal_chars,
                        const std::string& word, const std::vector<int>& pi) {
  // Jump over prefixes and add "illegal" letters to `illegal_chars`.
  for (int i = pi.back(); i > 0; i = pi[i - 1]) {
    illegal_chars.insert(word[i]);
  }
  illegal_chars.insert('a');
}

char GetMinCorrectChar(const std::unordered_set<char>& illegal_chars,
                       const std::vector<char>& alphabet) {
  for (char ch : alphabet) {
    if (illegal_chars.count(ch) == 0) {
      return ch;
    }
  }
}

char NextChar(const std::string& word, int value, const std::vector<int>& pi,
              const std::vector<char>& alphabet) {
  if (value == 0) {
    std::unordered_set<char> illegal_chars;
    FindIllegalLetters(illegal_chars, word, pi);
    return GetMinCorrectChar(illegal_chars, alphabet);
  } else {
    return word[value - 1];
  }
}

int main() {
  std::vector<char> alphabet(26);
  FillAlphabet(alphabet);

  std::string word;
  std::vector<int> pi;

  int value;
  std::cin >> value;  // Skip first value.
  word += 'a';
  pi.push_back(0);
  std::cout << word;

  while (std::cin >> value) {
    word += NextChar(word, value, pi, alphabet);
    pi.push_back(value);
    std::cout << word.back();
  }

  return 0;
}

