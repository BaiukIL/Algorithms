/*
 Дано число N и последовательность из N целых чисел.
 Найти вторую порядковую статистику на заданных диапазонах.
 Для решения задачи используйте структуру данных Sparse Table.
 Требуемое время обработки каждого диапазона O(1).
 Время подготовки структуры данных.

Формат ввода
  В первой строке заданы 2 числа: размер последовательности N и количество диапазонов M.
  Следующие N целых чисел задают последовательность.
  Далее вводятся M пар чисел - границ диапазонов.
  Гарантируется, что каждый диапазон содержит как минимум 2 элемента.
Формат вывода

 Для каждого из M диапазонов напечатать элемент последовательности - 2ю порядковую статистику.
 По одному числу в строке.
 */


#include <iostream>
#include <vector>
#include "sparse_table.h"

int main() {
  uint sequenceNumber, queriesNumber;
  std::cin >> sequenceNumber >> queriesNumber;
  std::vector<int> sequence;
  for (uint i = 0; i < sequenceNumber; ++i) {
    int element;
    std::cin >> element;
    sequence.push_back(element);
  }

  SparseTable ST(sequence);
  for (uint i = 0; i < queriesNumber; ++i) {
    int left, right;
    std::cin >> left >> right;
    std::cout << ST.GetMin(left - 1, right - 1) << '\n';
  }

  return 0;
}
