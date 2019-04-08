/* В большой IT-фирме есть только одна переговорная комната.
 * Желающие посовещаться заполняют заявки с желаемым временем начала и конца.
 * Ваша задача определить максимальное количество заявок, которое может быть удовлетворено.
 * Число заявок ≤ 100000.
 *
 * Формат входных данных:
 * Вход содержит только пары целых чисел — начала и концы заявок.
 *
 * Формат выходных данных:
 * Выход должен содержать натуральное число — максимальное число заявок.*/

#include <iostream>
#include <vector>
#include <algorithm>

struct Time {
    int start = 0;
    int end = 0;
};
//определим функцию сравнения следующим образом: отрезок а меньше отрезка b, если его конец меньше конца b
//если же концы равны, то а меньше b, если начало а не больше начала b
bool Compare (const Time &a, const Time &b) {
    return a.end == b.end ? a.start > b.start : a.end < b.end;
}
int Choose (std::vector <Time> &applications) {
//    сортируем отрезки по возрастанию концов
    std::sort (applications.begin(), applications.end(), Compare);
//    конец последнего взятого в качестве подходящего отрезка
    int last_end = -1;
//    счётчик подходящих отрезков
    int count = 0;
//    жадный алгоритм: берём отрезок с наименьшим концом (если его можно взять, т.е. если его начало меньше конца последнего
    for (auto &i : applications) {
        if (last_end <= i.start) {
            last_end = i.end;
            ++count;
        }
    }
    return count;
}
int main() {
    Time current;
    std::vector <Time> applications;
    while (std::cin >> current.start >> current.end) {
        applications.push_back (current);
    }
    std::cout << Choose (applications);
    return 0;
}
