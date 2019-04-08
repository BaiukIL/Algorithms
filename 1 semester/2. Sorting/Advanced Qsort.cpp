/* 
Дан массив целых чисел в диапазоне [0..109]. 
Размер массива кратен 10 и ограничен сверху значением 2.5 * 107 элементов. 
Все значения массива являются элементами псевдо-рандомной последовательности. 
Необходимо отсортировать элементы массива за минимально время и вывести 
каждый десятый элемент отсортированной последовательности.
Реализуйте сортировку, основанную на QuickSort.

Реализованные оптимизации QuickSort:
1. Оптимизация ввода/вывода
2. Оптимизация выбора опорного элемента: медиана трёх
3. Оптимизация Partition: проход с двух сторон
4. Без рекурсии
5. Оптимизация концевой рекурсии: сортировка вставками при длине куска менее 100
*/
#include <iostream>
#include <vector>
#include <stack>
// сортировка вставками
void InsertionSort(std::vector <int> &a, const int first, const int last)
{
	for (int i = first + 1; i <= last; ++i)
	{
		int temp = a[i];
		int j = i;
		for (; (j - 1) >= 0 && temp < a[j - 1]; --j)
		{
			a[j] = a[j - 1];
		}
		a[j] = temp;
	}
}
// медиана трёх
int Mid_Of_Three(const std::vector <int> &array, int first, int last)
{
	int a = array[first];
	int b = array[(first + last) / 2];
	int c = array[last];
	if (a <= b)
	{
		if (b <= c)
		{
			return (first + last) / 2;
		}
		else
		{
			if (a <= c)
				return last;
			else
				return first;
		}
	}
	else
	{
		if (b <= c)
		{
			if (a <= c)
				return first;
			else
				return last;
		}
		else
		{
			return (first + last) / 2;
		}
	}
}
void Partition(std::vector <int> &array, const int first, const int last, int& left, int& right)
{
	// i идёт от начала к концу, j от конца к началу
	int i = first;
	int j = last - 1;
	int size = last - first + 1;
	// ищем номер медианы трёх
	int number = Mid_Of_Three(array, first, last);
	// меняем местами выбранный (опорный) элемент с последним
	std::swap(array[number], array[last]);
	int pivot = array[last];
	
	while (i <= j)
	{
		// ищем первый элемент больше пивота
		for (; array[i] <= pivot && i < last; ++i) {}
		// ищем первый элемент не больше пивота
		for (; array[j] > pivot && j >= first; --j) {}
		// меняем их местами
		if (i < j)
		{
			std::swap(array[i++], array[j--]);
		}
	}
	// возвращаем пивот на соответствующее место
	std::swap(array[i], array[last]);
	// возвращаем концы разделенных отрезков
	left = i - 1;
	right = i + 1;
}
void QuickSort(std::vector <int> &array)
{
	// стек, хранящий начала и концы необработанных отрезков
	std::stack <int> unsorted;
	unsorted.push(0);
	unsorted.push(0);
	// first - начало отрезка, last - конец
	int first = 0, last = array.size() - 1;
	// left и right - концы левого и правого обработанных отрезков (то есть 
	// номера элементов справа и слева от пивота, которые далее станут началом/концом 
	// следующего отрезка)
	int left = 0, right = 0;
	// пока в стеке есть необработанные отрезки
	while (!unsorted.empty())
	{
		// пока длина отрезка больше 100
		while ((last - first + 1) > 100)
		{
			// вызываем Partition, передаём left и right, которые функция вернёт
			// как результат (через &)
			Partition(array, first, last, left, right);
			// если левый отрезок после обработки меньше правого, то помещаем правый в стек
			if ((left - first) < (last - right))
			{
				unsorted.push(right);
				unsorted.push(last);
				// меняем конец левого отрезка
				last = left;
			}
			// иначе помещаем левый в стек
			else
			{
				unsorted.push(first);
				unsorted.push(left);
				// меняем начало правого отрезка
				first = right;
			}
		}
		// длина стала меньше 100 => вызываем сортировку вставками
		InsertionSort(array, first, last);
		// если стек не пуст, то вынимаем концы лежащего (неотсортированного) отрезка
		if (!unsorted.empty())
		{
			last = unsorted.top();
			unsorted.pop();
			first = unsorted.top();
			unsorted.pop();
		}
	}
}
// выводим массив
void Print(const std::vector <int> &array)
{
	for (int i = 9; i < array.size(); i+=10)
	{
		std::cout<<array[i]<<' ';
	}
} 
int main()
{
	srand(time(NULL));
	int element;
	std::vector <int> array;
	std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
	for (; std::cin>>element;)
	{
		array.push_back(element);
	}
	QuickSort(array);
	Print(array);
	return 0;
}
