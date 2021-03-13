/*
 * Here you can see string convertion to Z-function.
 * This has passed A task (1.1) as OK which means that convertion works well.
 */


#include <iostream>
#include <vector>
#include <string>

// Required convertion from str to Z-function.
void BuildZfunction(const std::string& text, std::vector<int>& z) {
  int left = 0, right = 0;
  z[0] = 0;

  for (int pos = 1; pos < text.size(); ++pos) {
    if (pos < right) {
      if (pos + z[pos - left] < right) {
        z[pos] = z[pos - left];
      }
      else {
        int j = 0;

        while (text[(right - pos) + j] == text[right + j]) {
          ++j;
        }

        z[pos] = (right - pos) + j;
      }
    } else {
      int j = 0;

      while (text[j] == text[pos + j]) {
        ++j;
      }

      z[pos] = j;
    }

    if (pos + z[pos] > right) {
      left = pos;
      right = pos + z[pos];
    }
  }
}

// Uses in A task.
void GetSampleMatchPos(const std::string& sample, const std::string& text, std::vector<int>& positions) {
  std::string full_text = sample + "#" + text;
  std::vector<int> z(full_text.size());
  BuildZfunction(full_text, z);
  for (int i = 0; i < z.size(); ++i) {
    if (z[i] == sample.size()) {
      positions.push_back(i - (sample.size() + 1));
    }
  }
}

// Uses in A task.
int main() {
  std::string sample;
  std::cin >> sample;
  std::string text;

  std::cin >> text;

  std::vector<int> positions;

  GetSampleMatchPos(sample, text, positions);

  for (int i : positions) {
    std::cout << i << ' ';
  }

  return 0;
}

