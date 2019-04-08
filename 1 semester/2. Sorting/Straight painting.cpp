/* На числовой прямой окрасили N отрезков.
Известны координаты левого и правого концов каждого отрезка [Li, Ri]. 
Найти длину окрашенной части числовой прямой.
N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 109]. 
Выведите целое число — длину окрашенной части.*/

#include <iostream>

class Line
{
public:
	int L;
	int R;
};
template <typename T>
void swap(T &a, T &b)
{
	T c = a;
	a = b;
	b = c;
}
class Heap
{
	Line* buffer;
	int buffer_size;
	int tail;
public:
	Heap(int N)
	{
		buffer_size = N;
		buffer = new Line [N];
		tail = 0;
	}
	
	void SiftDown(int index)
	{
		int son_1 = index*2 + 1;
		int son_2 = index*2 + 2;
		int largest = index;
		while (son_1 < tail)
		{

			if (buffer[index].L <= buffer[son_1].L)
			{
				if (buffer[index].L == buffer[son_1].L)
				{
					if (buffer[index].R < buffer[son_1].R)
						largest = son_1;
				}
				else
					largest = son_1;
			}
			if (son_2 < tail)
			{
				if (buffer[largest].L <= buffer[son_2].L)
				{
					if (buffer[largest].L == buffer[son_2].L)
					{
						if (buffer[largest].R < buffer[son_2].R)
							largest = son_2;
					}
					else
						largest = son_2;	
				}
			}
			if (largest != index)
			{
				swap(buffer[index], buffer[largest]);
				index = largest;	
				son_1 = index*2 + 1;
				son_2 = index*2 + 2;
			}
			else 
				son_1 = tail;
		}
	}
	void Add(Line element)
	{
		buffer[tail++] = element;
	}
	void BuildHeap()
	{
		for (int i = buffer_size / 2 - 1; i >= 0; --i)
		{
			SiftDown(i);
		}	
	}
	void HeapSort()
	{
		while (tail != 0)
		{
			swap(buffer[0], buffer[--tail]);
			SiftDown(0);
		}
	}
	int count()
	{
		int result = 0;
		int min = buffer[0].L, max = buffer[0].R;
		for (int i = 1; i < buffer_size; ++i)
		{
			int	a = buffer[i].L, b = buffer[i].R;
			if (a > max)
			{
				result += max - min;
				min = a;
				max = b;
			}
			else 
			{
				if (b > max)
					max = b;
			}
		}
		result += max - min;
		return result;
	}
	~Heap()
	{
		delete [] buffer;
	}
};

int main()
{
    int N;
    std::cin>>N;
	Heap arr(N);
    for (int i = 0; i < N; ++i)
	{
		Line a;
        std::cin>>a.L>>a.R;
		arr.Add(a);
	}
	arr.BuildHeap();
	arr.HeapSort();
	std::cout<<arr.count();
    return 0;
}
