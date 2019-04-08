/*

incorrect solution!!!

Дан массив неотрицательных целых 64-разрядных чисел. 
Количество чисел не больше 1000000. 
Отсортировать массив методом MSD по битам (бинарный QuickSort).
 */
#include <iostream>
#include <vector>
#include <stack>

void Partition(std::vector <int64_t> &array, int dif, const int first, const int last, int& left, int& right)
{
	// i идёт от начала к концу, j от конца к началу
	int i = first;
	int j = last;
	// задаём пивот = 1
	int64_t pivot = 1;
	while (i <= j)
	{
		// ищем первый элемент больше пивота, сдвигаем элемент на dif разрядов
		for (; ((array[i] >> dif) & 1) != pivot && i <= last; ++i) {}
		// ищем первый элемент не больше пивота
		for (; ((array[j] >> dif) & 1) == pivot && j >= first; --j) {}
		// меняем их местами
		if (i < j)
		{
			std::swap(array[i++], array[j--]);
		}
	}
	// если все сдвинутые на dif элементы оказались 0, то оставляем границу (right не обрабатывается)
	if (i > last)
	{
		left = last;
		right = last + 1;
	}
	// иначе делим на две части
	else
	{
		left = i - 1;
		right = i;
	}
}
void MSDsort(std::vector <int64_t> &array, int64_t max)
{
	int dif = 0;
	// находим количество разрядов максимального элемента (dif + 1) 
	// сдвигать будем на dif
	for (; max > 1; dif++)
	{
		max /= 2;
	}
	// стек, хранящий начала и концы необработанных отрезков, а также dif
	std::stack <int64_t> unsorted;
	unsorted.push(0);
	unsorted.push(0);
	unsorted.push(0);
	// first - начало отрезка, last - конец
	int first = 0, last = array.size() - 1;
	// left и right - концы левого и правого обработанных отрезков
	int left = 0, right = 0;
	// пока в стеке есть необработанные отрезки
	while (!unsorted.empty())
	{
		// пока в сортируемом отрезке более одного элемента
		while ((last > first) && (dif >= 0))
		{
			// вызываем Partition, передаём left и right, которые функция вернёт как результат (через &)
			Partition(array, dif--, first, last, left, right);
			// если левый отрезок после обработки меньше правого, то помещаем правый в стек
			if ((left - first) < (last - right))
			{
				unsorted.push(right);
				unsorted.push(last);
				unsorted.push(dif);
				// меняем конец левого отрезка
				last = left;
			}
			// иначе помещаем левый в стек
			else
			{
				unsorted.push(first);
				unsorted.push(left);
				unsorted.push(dif);
				// меняем начало правого отрезка
				first = right;
			}
		}
		// если стек не пуст, то вынимаем концы лежащего (неотсортированного) отрезка и dif
		if (!unsorted.empty())
		{
			dif = unsorted.top();
			unsorted.pop();
			last = unsorted.top();
			unsorted.pop();
			first = unsorted.top();
			unsorted.pop();
		}
	}
}
// вывод массива
void Print(const std::vector <int64_t> &array)
{
	for (int i = 0; i < array.size(); ++i)
	{
		std::cout<<array[i]<<' ';
	}
}
int main()
{
	int N;
	std::cin >> N;
	// максимальный элемент
	int64_t max = 0;
	std::vector <int64_t> array;
	for (int i = 0; i < N; ++i)
	{
		int64_t element;
		std::cin >> element;
		array.push_back(element);
		// параллельно ищем максимальный
		if (element > max)
			max = element;
	}
	MSDsort(array, max);
	Print(array);
	return 0;
}
