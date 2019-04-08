/*
 Дана последовательность целых чисел из диапазона (-1000000000 .. 1000000000).
 Длина последовательности не больше 1000000.
 Числа записаны по одному в строке. Количество чисел не указано.
 Пусть количество элементов n, и числа записаны в массиве a = a[i]: i из [0..n-1].
 Требуется напечатать количество таких пар индексов (i,j) из [0..n-1], что (i < j и a[i] > a[j]).
 Указание: количество инверсий может быть больше 4*1000000000 - используйте int64_t.
*/

#include <iostream>
#include <vector>

// Merge(из какого вектора берутся части, номер первого элемента левой части, длина левой части,
// длина правой части (т. к. сливаемые части следуют друг за другом, то номер первого элемента
// вводить не нужно), счётчик инверсий, массив в который происходит сливание)
void Merge(std::vector <int64_t> &mass, int L_first, int L_length, int R_length, int64_t &count, std::vector <int64_t> &result)
{
    int R_first = L_first + L_length;   //номер первого элемента правой части

    int64_t local_count = 0;    //локальный счётчик инверсий

    int i = L_first;    //иттераторы
    int j = R_first;

    int L_last = L_first + L_length - 1; //последние элементы левой и правой частей
    int R_last = R_first + R_length - 1;

    while (i <= L_last && j <= R_last)
    {
        //если элемент из левой части больше элемента из правой, то ставим последний в result
        if (mass[i] > mass[j])
        {
            result.push_back(mass[j++]);
            ++ local_count;
        }
        else    //иначе ставим элемент из левой части
        {
            result.push_back(mass[i++]);
            count += local_count;
        }
    }
    //когда одна из частей кончилась, перемещаем оставшуюся в result
    while (i <= L_last)
    {
        result.push_back(mass[i++]);
        count += local_count;
    }
    while (j <= R_last)
    {
        result.push_back(mass[j++]);
    }
}
int64_t MergeSort(std::vector <int64_t> &mass)
{
    int length = 1; //длина отрезков (сливаемых частей)

    int64_t count = 0;  //счётчик инверсий, возвращается как результат работы функции

    std::vector <int64_t> result;   //вектор, в котором происходит сливание
    while (length < mass.size())
    {
        result.clear(); //очищаем от предыдущих элементов (появились на предыдущей иттерации цикла)
        //current - номер текущего элемента
        for (int current = 0; current < mass.size(); current += length * 2)
        {
            //если далее достаточно места для двух отрезков длины length, то сливаем их
            if (current + length * 2 - 1 < mass.size())
            {
                Merge(mass, current, length, length, count, result);
            }
        }
        //копируем result в mass, проходим следующий цикл с изменённым mass
        for (int i = 0; i < result.size(); ++i)
        {
            mass[i] = result[i];
        }
        /*std::copy или assing использовать не получится, т. к. в этих случаях
         * заменяется весь вектор, нам же надо сохранить неотсортированную часть в mass*/
        length *= 2;    //увеличиваем дальнейшую длину отрезков в 2 раза
    }

    int sorted = length / 2; //размер отсортированной части
    //сливаем оставшиеся на конце куски с отсортированным отрезком
    while (sorted != mass.size())
    {
        length /= 2;
        result.clear();
        //если отрезок длины length влезает в
        // неотсортированную часть массива, то сливаем его с главным
        if (sorted + length <= mass.size())
        {
            Merge(mass, 0, sorted, length, count, result);
            sorted += length;
            for (int i = 0; i < result.size(); ++i)
            {
                mass[i] = result[i];
            }
        }
    }
    return count;
}

int main()
{
    int64_t element;
    std::vector <int64_t> mass;
    for (; std::cin>>element;)
    {
        mass.push_back(element);
    }

    std::cout<<MergeSort(mass);

    return 0;
}
