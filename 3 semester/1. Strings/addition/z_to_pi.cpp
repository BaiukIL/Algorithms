#include <iostream>
#include <vector>

// Tested on Task 1.2 (B2) for correctness.
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

int main() {
  std::vector<int> z;

  int z_pos = 0;

  std::cout << "Wait for correct Z-function (enter Ctrl + d to finish):\n";

  while (std::cin >> z_pos) {
    z.push_back(z_pos);
  }

  for (int i : ConvertZToPi(z)) {
    std::cout << i << ' ';
  }

  return 0;
}

