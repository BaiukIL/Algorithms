/*
 * Дано число N < 106 и последовательность пар целых чисел из [-231, 231] длиной N.
 * Построить декартово дерево из N узлов, характеризующихся парами чисел (Xi, Yi).
 * Каждая пара чисел (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом дереве.
 * Добавление узла в декартово дерево выполняйте второй версией алгоритма, рассказанного на лекции:
 * При добавлении узла (x, y) выполняйте спуск по ключу до узла P с меньшим приоритетом.
 * Затем разбейте найденное поддерево по ключу x так, чтобы в первом поддереве
 * все ключи меньше x, а во втором больше или равны x.
 * Получившиеся два дерева сделайте дочерними для нового узла (x, y).
 * Новый узел вставьте на место узла P. Построить также наивное дерево поиска по ключам Xi.
 * Равные ключи добавляйте в правое поддерево.
 * Вычислить количество узлов в самом широком слое декартового дерева и количество узлов
 * в самом широком слое наивного дерева поиска.
 * Вывести их разницу. Разница может быть отрицательна.
*/
#include <iostream>
#include <stack>
#include <queue>

template <typename T>
class Node {
public:
    T key;
    int priority;
    Node* left;
    Node* right;
    explicit Node(T key, int priority):
            key(key),
            priority(priority),
            left(nullptr),
            right(nullptr) {}
};

template <typename M>
class Treap {
    Node <M>* root;
//    операция разрезания дерева с вершиной в узле current по ключу element;
//    возвращает left и right - корни левого и правого разрезанных деревьев соответственно
    void split(Node <M>* current, const int &element, Node <M>* &left, Node <M>* &right) {
        if (current == nullptr) {
            left = nullptr;
            right = nullptr;
        } else {
            if (element <= current->key) {
                split(current->left, element, left, current->left);
                right = current;
            } else {
                split(current->right, element, current->right, right);
                left = current;
            }
        }
    }
//    вставка узла в дерево
    Node <M>* insert(Node <M>* &current, Node <M>* &node) {
//        если дошли до листа (nullptr), то вставляем в него node
        if (current == nullptr){
            return node;
        }
//        рекурсивно спускаемся по ключам, пока не найдём узел с меньшим приоритетом.
        if (current->priority > node->priority){
            node->key < current->key ? current->left = insert(current->left, node) : current->right = insert(current->right, node);
            return current;
        }
//        нашли - разрезаем дерево с корнем в этом узле по ключу node->key, далее согласно
//        инструкции к задаче
        else{
            Node <M>* left = nullptr;
            Node <M>* right = nullptr;
            split(current, node->key, left, right);
            node->left = left;
            node->right = right;
            return node;
        }
    }
public:
//    конструктор - создаём "пустой" корень
    Treap(): root(nullptr) {}
    void insertion(const int &value, const int &priority){
//        создаём узел со значениями value и priority, который и будем вставлять в дерево
        auto * node = new Node <M> (value, priority);
//        если корень есть (и не пуст), то вставляем узел в него с помощью insertion
//        иначе делаем этот узел корнем
        root == nullptr ? root = node : root = insert(root, node);
    }
//    макс. ширина дерева
    int BFS_length() {
//        очередь необработанных элементов
        std::queue <Node <M>*> uncounted;
        uncounted.push(root);
        Node <M>* current;
        int this_level = 1; //счётчик узлов на текущем уровне дерева
        int next_level = 0; //счётчик узлов на сделующем уровне дерева
        int max_length = 0; //максимальная ширина
//        пока стек не пуст
        while (!uncounted.empty()) {
//            вынимаем узел
            current = uncounted.front();
            uncounted.pop();
            --this_level;
//            если есть правый узел, то кладём в очередь, увеличиваем кол-во узлов на следующем уровне
            if (current->left != nullptr) {
                uncounted.push(current->left);
                ++next_level;
            }
//            есть левый - кладём в очередь, увеличиваем кол-во узлов на следующем уровне
            if (current->right != nullptr) {
                uncounted.push(current->right);
                ++next_level;
            }
//            если узлы на текущем уровне закончились, то сравниваем кол-во узлов
//            на следующем уровне с max_length. Делаем текущий уровень следующим и обнуляем
//            счётчик кол-ва узлов на следующем уровне
            if (this_level == 0) {
                if (next_level > max_length) {
                    max_length = next_level;
                }
                this_level = next_level;
                next_level = 0;
            }
        }
        return max_length;
    }
    ~Treap() {
//        заводим стек необработанных узлов
        std::stack <Node <M>*> undeleted;
        undeleted.push(root);
        Node <M>* current;
//        продолжаем, пока не закончатся необработанные узлы
        while (!undeleted.empty()) {
            current = undeleted.top();  //вынимаем из стека необработанные
            undeleted.pop();
            Node <M>* tmpL = current->left;  //левый и правый подузлы текущего
            Node <M>* tmpR = current->right;
//            если есть левый подузел, то кладём его в стек
            if (tmpL != nullptr){
                undeleted.push(tmpL);
            }
//            если есть правый подузел, то кладём его в стек
            if (tmpR != nullptr){
                undeleted.push(tmpR);
            }
//            удаляем текущий узел
            delete current;
        }
    }
};

template <typename T>
//узел наивного дерева поиска
struct Node_simple {
    T key;
    Node_simple* left = nullptr;
    Node_simple* right = nullptr;
    explicit Node_simple(T key):
            key(key),
            left(nullptr),
            right(nullptr) {}
};

template <typename M>
//наивное дерево поиска
class Tree {
    Node_simple <M>* root;
public:
    Tree(): root(nullptr) {}
//   вставка элемента
    void insertion(const int &value) {
        if (root == nullptr) {
            root = new Node_simple <M> (value);
            return;
        }
        Node_simple <M>* current = root; //создаём "текущий" узел, которым будем проходить по девеву
        bool check = true; //индикатор, пока элемент не добавился в кучу равен true
        while (check) {
            if (value < current->key) {
                if (current->left == nullptr) {
                    current->left = new Node_simple <M> (value);
                    check = false;
                } else {
                    current = current->left;
                }
            } else {
                if (current->right == nullptr) {
                    current->right = new Node_simple <M> (value);
                    check = false;
                } else {
                    current = current->right;
                }
            }
        }
    }
//    макс. ширина; реализовано аналогично Treap
    int BFS_length() {
        std::queue <Node_simple <M>*> uncounted;
        uncounted.push(root);
        Node_simple <M>* current;
        int this_level = 1;
        int next_level = 0;
        int max_length = 0;
        while (!uncounted.empty()) {
            current = uncounted.front();
            uncounted.pop();
            --this_level;
            if (current->left != nullptr) {
                uncounted.push(current->left);
                ++next_level;
            }
            if (current->right != nullptr) {
                uncounted.push(current->right);
                ++next_level;
            }
            if (this_level == 0) {
                if (next_level > max_length) {
                    max_length = next_level;
                }
                this_level = next_level;
                next_level = 0;
            }
        }
        return max_length;
    }
//    деструктор
    ~Tree(){
//        заводим стек необработанных узлов
        std::stack <Node_simple <M>*> undeleted;
        undeleted.push(root);
        Node_simple <M>* current;
//        продолжаем, пока не закончатся необработанные узлы
        while (!undeleted.empty()) {
            current = undeleted.top();  //вынимаем из стека необработанные
            undeleted.pop();
            Node_simple <M>* tmpL = current->left;  //левый и правый подузлы текущего
            Node_simple <M>* tmpR = current->right;
//            если есть левый подузел, то кладём его в стек
            if (tmpL != nullptr){
                undeleted.push(tmpL);
            }
//            если есть правый подузел, то кладём его в стек
            if (tmpR != nullptr){
                undeleted.push(tmpR);
            }
//            удаляем текущий узел
            delete current;
        }
    }
};

int main() {
    Treap <int> treap;
    Tree <int> tree;
    int N, x, y;
    std::cin>>N;
    for (int i = 0; i < N; ++i){
        std::cin>>x>>y;
        treap.insertion(x, y);
        tree.insertion(x);
    }
    std::cout<<treap.BFS_length() - tree.BFS_length();
    return 0;
}
