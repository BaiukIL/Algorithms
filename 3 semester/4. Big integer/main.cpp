#include <iostream>
#include <cassert>
#include "biginteger.h"


int main() {
  BigInteger a, b;
  std::cin >> a >> b;
  std::cout << "a: " << a << std::endl;
  std::cout << "b: " << b << std::endl;
  std::cout << "a + b: " << (a + b) << std::endl;
  std::cout << "a - b: " << (a - b) << std::endl;
  std::cout << "a * b: " << (a * b) << std::endl;
  std::cout << "a / b: " << (a / b) << std::endl;
  std::cout << "a \% b: " << (a % b) << std::endl;
  return 0;
}

