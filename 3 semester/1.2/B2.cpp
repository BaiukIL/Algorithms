#include <iostream>
#include <unordered_set>
#include <vector>

std::vector<int> ConvertZToPi(const std::vector<int>& z) {
  std::vector<int> pi(z.size(), 0);

  for (int i = 1; i < z.size(); ++i) {
    for (int j = z[i] - 1; j >= 0; --j) {
      if (pi[i + j] > 0) {
        break;
      } else {
        pi[i + j] = j + 1;
      }
    }
  }

  return pi;
}

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
  // Convert Z to pi.
  std::vector<int> z;

  for (int value; std::cin >> value; ) {
    z.push_back(value);
  }

  // Bring to B1.
  std::vector<int> pi_full = ConvertZToPi(z);

  std::vector<char> alphabet(26);
  FillAlphabet(alphabet);

  std::string word;

  std::vector<int> pi;

  word += 'a';
  std::cout << word;
  pi.push_back(0);

  for (int i = 1; i < pi_full.size(); ++i) {
    int value = pi_full[i];
    word += NextChar(word, value, pi, alphabet);
    pi.push_back(value);
    std::cout << word.back();
  }

  return 0;
}

