/*Дано N кубиков.
 Требуется определить каким количеством способов можно выстроить из этих кубиков пирамиду.
 Каждый вышележащий слой пирамиды должен быть не больше нижележащего.


 Формат ввода
 На вход подается количество кубиков N.

 Формат вывода
 Вывести число различных пирамид из N кубиков.*/

#include <iostream>
#include <vector>

long Combinations (const unsigned int &N, const int &H, std::vector <std::vector <long>> &Cache) {
//    если ячейка уже обрабатывалась на каком-то шаге (то есть если её значение не -1),
//    то не нужно заново вычислять её - возвращаем записанный в ней результат
    if (Cache[N][H] != -1) {
        return Cache[N][H];
    }
//    Пусть P(N, H) - кол-во пирамид, составляемых из N кубиков c начальным слоем H.
//    Тогда рекуррентная формула имеет вид: P(N, H) = P(N, H - 1) + P(N - H, H).
//    Результаты каждого нового шага записываем в кэш-таблицу
    if (H <= N) {
        return Cache[N][H] = Combinations (N, H - 1, Cache) + Combinations (N - H, H, Cache);
    }
    else {
        return Cache[N][H] = Combinations (N, H - 1, Cache);
    }
}
long Pyramids_count (const unsigned int &N) {
//    кэш-таблица [0, N] x [0, N], в которую будем записывать результаты каждого шага
    std::vector <std::vector <long>> Cache (N + 1);
//    заполняем первую строку таблицы нулями
    for (int i = 0; i <= N; ++i) {
        Cache[i].resize(N + 1, -1);
        Cache[i][0] = 0;
    }
//    первый столбец заполняем 1
    for (int i = 0; i <= N; ++i) {
        Cache[0][i] = 1;
    }
    return Combinations(N, N, Cache);
}
int main() {
    unsigned int N;
    std::cin >> N;
    std::cout << Pyramids_count(N);
    return 0;
}
