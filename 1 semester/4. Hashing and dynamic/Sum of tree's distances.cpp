/*Дано невзвешенное дерево.
 Расстоянием между двумя вершинами будем называть количество ребер в пути, соединяющем эти две вершины.
 Для каждой вершины определите сумму расстояний до всех остальных вершин.

 Время работы должно быть O(n).

 Формат ввода
 В первой строке записано количество вершин n ≤ 10000.
 Затем следует n - 1 строка, описывающая ребра дерева.
 Каждое ребро - это два различных целых числа - индексы вершин в диапазоне [0, n-1].
 Индекс корня – 0. В каждом ребре родительской вершиной является та, чей номер меньше.

 Формат вывода
 Выход должен содержать n строк.
 В i-ой строке выводится сумма расстояний от i-ой вершины до всех остальных.*/
#include <iostream>
#include <vector>
#include <stack>

class Node {
public:
//    сумма расстояний до узлов, расположенных ниже данного
    int sum;
//    кол-во узлов под данным
    int number_under;
//    индикатор, требуется для DFS-обхода
    bool checked;
//    вектор номеров детей узла
    std::vector <int> children;
    explicit Node ():
        sum (0),
        number_under (0),
        children (0),
        checked (false) {}
};
void Set_child (std::vector <Node> &tree, const int &parent, const int &child) {
    tree[parent].children.push_back (child);
}
//обход дерева для поиска сумм и кол-ва узлов, необходимых для работы Sum_count
void Build_tree (std::vector <Node> &tree) {
//    стек номеров необработанных узлов
    std::stack <int> DFS;
//    push root number
    DFS.push (0);
    for (int i; !DFS.empty(); ) {
        i = DFS.top ();
//        если узел был проверен, то все его дети обработаны, сл-но, можем обрабатывать сам узел
        if (tree[i].checked) {
            for (auto j : tree[i].children) {
                tree[i].sum += tree[j].sum + tree[j].number_under + 1;
                tree[i].number_under += tree[j].number_under + 1;
            }
            DFS.pop ();
        } else {
//            если у узла есть дети, то помещаем их в стек и помечаем узел проверенным
            if (!tree[i].children.empty ()) {
                for (auto j : tree[i].children) {
                    DFS.push (j);
                }
                tree[i].checked = true;
            } else {
//                иначе (если узел листовой), то обрабатываем, удаляем его номер из стека
                tree[i].sum = 0;
                tree[i].number_under = 0;
                DFS.pop ();
            }
        }
    }
}
int Step (const int &N, const int &S, const Node &current) {
//    полная формула:
//    сумма_расстояний = sum + сумма_расстояний_от_родителя_до_остальных_без_учёта_путей_через_даный_узел + (общее_кол-во_узлов - (кол-во_узлов_под_текущим + текущий))
//    сокращённый вид = N (общее кол-во узлов) + S (сумма_расстояний_от_родителя_до_остальных) - 2 * (кол-во_узлов_под_текущим + 1)
    return N + S - 2 * (current.number_under + 1);
}
void Sum_count (std::vector <Node> &tree, std::vector <int> &final_sum, const unsigned int &N) {
    Build_tree (tree);
//    для корня результат и есть его sum
    final_sum[0] = tree[0].sum;
    for (int i = 0; i < N; ++i) {
        for (auto &j : tree[i].children) {
            final_sum[j] = Step (N, final_sum[i], tree[j]);
        }
    }
}
int main() {
    unsigned int N;
    std::cin >> N;
    std::vector <Node> tree (N);
//    в векторе хранятся итоговые суммы длин от узла i из [0, N) до остальных
    std::vector <int> final_sum (N);
//    считывание
    for (int i = 0; i < N - 1; ++i) {
        int parent, child;
        std::cin >> parent >> child;
        Set_child(tree, parent, child);
    }
    Sum_count (tree, final_sum, N);
//    вывод
    for (int i : final_sum) {
        std::cout << i << '\n';
    }
    return 0;
}
