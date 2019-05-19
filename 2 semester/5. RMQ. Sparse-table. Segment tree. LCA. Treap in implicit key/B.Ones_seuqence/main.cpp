/*
  Дан массив из нулей и единиц a0, a1, ..., an-1.
 Для каждого запроса [left, right] найдите такой подотрезок al, al+1, ..., ar этого массива
 (0 <= left <= l <= r <= right < n), что числа al, al+1, …, ar являются
 максимально возможной последовательностью единиц.
 Требуемое время ответа на запрос - O(log n).

  Формат ввода
 Описание каждого теста начинается с двух чисел n и m - длины массива и числа интересующих подотрезков.
 В следующей строке содержится n нулей и единиц.
 Далее следуют описания подотрезков, каждое описание состоит из двух чисел left и right,
 обозначающих левый и правый конец подотрезка (0 <= left <= right < n).

  Формат вывода
 Для каждого примера выведите m чисел:
 искомую максимальную длину послеовательности единиц для каждого из подотрезков.
 */

#include <iostream>
#include <vector>
#include "SegmentTree.h"


int main() {
  int sequenceNumber, queriesNumber;
  std::cin >> sequenceNumber >> queriesNumber;
  std::vector<int> binSequence;
  for (int i = 0; i < sequenceNumber; ++i) {
    int element;
    std::cin >> element;
    binSequence.push_back(element);
  }

  SegmentTree ST(binSequence);
  for (int i = 0; i < queriesNumber; ++i) {
    int left, right;
    std::cin >> left >> right;
    std::cout << ST.GetMaxUnitsNumber(left, right) << '\n';
  }

  return 0;
}
