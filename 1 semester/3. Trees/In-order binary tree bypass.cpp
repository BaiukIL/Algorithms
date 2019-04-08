/*
Дано число N ≤ 104 и последовательность целых чисел из [-231..231] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root,
если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Выведите элементы в порядке in-order (слева направо).
Рекурсия запрещена.
*/
#include <iostream>
#include <stack>

//узел
template<typename T>
struct Node {
  T key;
  Node *left;
  Node *right;
  explicit Node(T key) : key(key), left(nullptr), right(nullptr) {}
};

//бинарное дерево поиска
template<typename T>
class BinaryTree {
private:
    Node<T> *root; //корень
public:
    BinaryTree() : root(nullptr) {}

    //вставка элемента
    void insertion(const int &value);

    void in_order_print() const;

    ~BinaryTree();
};

template<typename T>
void BinaryTree<T>::insertion(const int &value) {
  if (root == nullptr) {
    root = new Node<T>(value);
    return;
  }
  Node<T> *current = root;   //создаём "текущий" узел, которым будем проходить по девеву
  bool check = true;  //индикатор. Пока элемент не добавился в кучу равен true
  while (check) {
    if (value < current->key) {
      if (current->left == nullptr) {
        current->left = new Node<T>(value);
        check = false;
      } else {
        current = current->left;
      }
    } else {
      if (current->right == nullptr) {
        current->right = new Node<T>(value);
        check = false;
      } else {
        current = current->right;
      }
    }
  }
}

template<typename T>
void BinaryTree<T>::in_order_print() const {
  /* Алгоритм вывода:
     * Если у текущего узла существует левое поддерево,
     * то помещаем текущий в стек для дальнейшей обработки; делаем корень левого поддерева теущим
     * Таким образом мы найдём крайний слева узел. Нашли - выводим его и смотрим правое поддерево:
     * если оно существует, то делаем его корень текущим и выполняем алгоритм для этого поддерева
     * Если же правого поддерева нет, то вынимаем значение из стека и выводим его.
     * Заметим, что для каждого узла в стеке мы рассмотрели левые поддеревья, поэтому ещё раз
     * рассматривать их не нужно. Значит, смотрим на правое поддерево вынутого узла - если оно
     * существует, то делаем текущим корень правого поддерева. Если правого нет, то снова вынимаем
     * элемент из стека и делаем его текущим.
     * */
  std::stack<Node<T> *> unchecked;
  unchecked.push(nullptr);                    //"нулевой" элемент стека
  Node<T> *current = root;
  while (current != nullptr) {                //если текущий нулевой, то прекращаем
    while (current->left != nullptr) {        //ищем крайний левый
      unchecked.push(current);
      current = current->left;
    }
    std::cout << current->key << '\n';
    while (current->right == nullptr) {
      current = unchecked.top();
      unchecked.pop();
      if (current == nullptr) { return; }
      std::cout << current->key << '\n';  //иначе выводим его
      //повторяем, пока не найдётся правое поддерево и стек не пуст
    }
    current = current->right;   //нашли правое поддерево - делаем текущим
  }
}

template<typename T>
BinaryTree<T>::~BinaryTree() {
  std::stack<Node<T> *> non_deleted;  //заводим стек необработанных узлов
  non_deleted.push(root);
  Node<T> *current;
  //продолжаем, пока не закончатся необработанные узлы
  while (!non_deleted.empty()) {
    current = non_deleted.top();  //вынимаем из стека необработанные узлы
    non_deleted.pop();
    Node<T> *tmpL = current->left;  //левый и правый подузлы текущего
    Node<T> *tmpR = current->right;
    if (tmpL != nullptr)  //если есть левый подузел, то кладём его в стек
      non_deleted.push(tmpL);
    if (tmpR != nullptr)  //если есть правый подузел, то кладём его в стек
      non_deleted.push(tmpR);
    delete current;
  }
}

int main() {
  BinaryTree<int> tree;
  int N, key;
  std::cin >> N;
  for (int i = 0; i < N; ++i) {
    std::cin >> key;
    tree.insertion(key);
  }
  tree.in_order_print();
  return 0;
}
