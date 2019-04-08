/* Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. 
 * Хранимые строки непустые и состоят из строчных латинских букв. 
 * Начальный размер таблицы должен быть равным 8-ми. 
 * Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Хеш-функцию строки реализуйте с помощью вычисления значения многочлена методом Горнера.
 * Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству. 
 * Для разрешения коллизий используйте квадратичное пробирование. 
    i-ая проба g(k, i) = g(k, i-1) + i (mod m). m - степень двойки.
 
 * Формат входных данных
 * Каждая строка входных данных задает одну операцию над множеством. 
 * Запись операции состоит из типа операции и следующей за ним через пробел строки, над которой проводится операция.
    Тип операции  – один из трех символов:
    +  означает добавление данной строки в множество; 
    -  означает удаление  строки из множества;  
    ?  означает проверку принадлежности данной строки множеству.
 * При добавлении элемента в множество НЕ ГАРАНТИРУЕТСЯ, что он отсутствует в этом множестве.
 * При удалении элемента из множества НЕ ГАРАНТИРУЕТСЯ, что он присутствует в этом множестве.

 * Формат выходных данны
 * Программа должна вывести для каждой операции одну из двух строк OK или FAIL, в зависимости от того, встречается ли данное слово в нашем множестве.*/

#include <iostream>
#include <vector>

//коэффициент заполнения, при достижении которого необходимо увеличивать размер таблицы
constexpr double full_factor = 3.0 / 4.0;
//начальный размер таблицы
constexpr unsigned int initial_size = 8;

struct Pair {
    std::string str;
    bool deleted;
    Pair(): str(""), deleted(false) {}
};
int Hash (const std::string &str, const unsigned int &M, const unsigned int &hash_number) {
    int hash = 0;
    for (int i = 0; i != str.size(); ++i) {
        hash = (hash * hash_number + str[i]) % M;
    }
    return hash;
}
class Hash_Table {
public:
    Hash_Table(): capacity(initial_size), N(0), hash_number(initial_size - 1), table(initial_size) {}
    bool insert(std::string &str);
    bool remove(std::string &str);
    bool search(std::string &str) const;
private:
//    размер таблицы
    unsigned int capacity;
//    кол-во элементов в таблице
    unsigned int N;
//    число, взаимно простое с capacity; используется в хеш-функции
    unsigned int hash_number;
    std::vector <Pair> table;
//    второй конструктор нужен для функции rehash
    explicit Hash_Table (unsigned int new_size): capacity(new_size), N(0), hash_number(new_size - 1), table(new_size) {}
    void rehash();
};
//заводим новую таблицу вдвое большего розмера, поочерёдно добавляем в неё неудалённые элементы пререполненной таблицы
//копируем поля новой таблицы
void Hash_Table::rehash() {
    Hash_Table new_table (capacity * 2);
    for (int i = 0; i < capacity; ++i) {
        if (!table[i].str.empty() && !table[i].deleted) {
            new_table.insert(table[i].str);
        }
    }
    capacity *= 2;
    N = new_table.N;
    hash_number = new_table.hash_number;
    table = new_table.table;
}
//вставка
bool Hash_Table::insert(std::string &str) {
//    вычисляем хеш (индекс)
    int current = Hash(str, capacity, hash_number);
//    флаг: как только нашли подходящую для вставки ячейку, запоминаем её
    int value = -1;
//    пока не встретим пустой, проверяем начилие ячейки с таким же ключом
//    нашли, сл-но, такой ключ уже есть - возвращаем false
    for (int i = 0; !table[current].str.empty(); ) {
        if (table[current].deleted && value == -1) {
            value = current;
        }
        if (table[current].str == str && !table[current].deleted) {
            return false;
        }
//        квадратичное пробирование
        ++i;
        current = (current + i) % capacity;
    }
//    если не нашли ячейку с удалённым значением, то вставляем на место пустой ячейки
    if (value == -1){
        value = current;
    }
//    если в подходящей для вставки ячейке значение удалёно, то не увеличиваем N
    if (!table[value].deleted){
        ++N;
    }
    table[value].str = str;
    table[value].deleted = false;
//    увеличиваем размер таблицы, если она переполнилась
    if (N == capacity * full_factor){
        rehash();
    }
    return true;
}
//удаление
bool Hash_Table::remove (std::string &str) {
//    вычисляем хеш (индекс)
    int current = Hash(str, capacity, hash_number);
//    ищем до первой пустой ячейки
    for (int i = 0; !table[current].str.empty(); ) {
//        если встретили неудалённую ячейку с таким же значением, то такой ключ уже есть - возвращаем false
        if (!table[current].deleted && table[current].str == str) {
            table[current].deleted = true;
            return true;
        }
        ++i;
        current = (current + i) % capacity;
    }
    return false;
}
//поиск
bool Hash_Table::search (std::string &str) const {
//    вычисляем хеш (индекс)
    int current = Hash(str, capacity, hash_number);
//    если нашли неудалённую ячейку с таким же ключом, то такой ключ есть в таблице - выходим из цикла
    for (int i = 0; !(table[current].str == str && !table[current].deleted); ) {
//        если встретили пустую ячейку, то такого ключа нет - возвращаем false
        if (table[current].str.empty()) {
            return false;
        }
        ++i;
        current = (current + i) % capacity;
    }
    return true;
}

int main() {
    Hash_Table mass;
    char command;
    std::string str;
    bool result;
    while (std::cin >> command) {
        std::cin >> str;
        switch (command) {
            case '+':
                result = mass.insert(str);
                break;
            case '-':
                result = mass.remove(str);
                break;
            case '?':
                result = mass.search(str);
                break;
            default:
                return 1;
        }
        result ? std::cout << "OK\n" : std::cout << "FAIL\n";
    }
    return 0;
}
