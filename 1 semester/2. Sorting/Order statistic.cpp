/*
Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число,
которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве.
Напишите нерекурсивный алгоритм.
Требования к дополнительной памяти: O(n). Требуемое среднее время работы: O(n).
Функцию Partition следует реализовывать методом прохода
двумя итераторами в одном направлении.
*/

#include <iostream>
#include <ctime>

int Partition(int* a, int size, int k)
{
    int i = 0;
    int last = size - 1;
    while (true)
    {
        int first = i;
        int r_index = (rand() % (last - first)) + i;
        std::swap(a[r_index], a[last]);
    
        for (; a[i] <= a[last] && i < last; ++i) 
        {}	//сдвигаем i на первый элемент, больше опорного
    
        for (int j = i + 1; j < last; ++j)
        {
            if (a[j] <= a[last])
                std::swap(a[j], a[i++]);
        }
        std::swap(a[i], a[last]);	//возвращаем опорный на место
    
        if (k == i)
            return a[i];
        else
        {
            if (k < i)	//берём левый отрезок
            {
                last = i - 1;
                if (last == first)
                    return a[i - 1];
                i = 0;
            }
            else	//берём правый отрезок
            {
                ++i;
                if (i == last)
                    return a[i];
            }
        }
    }
}
int main()
{
    srand(time(NULL));
    int N, k;
    std::cin>>N>>k;
    int* a = new int [N];
    for (int i = 0; i < N; ++i)
    {
        std::cin>>a[i];
    }
    std::cout<<Partition(a, N, k);

    delete [] a;
    return 0;
}
