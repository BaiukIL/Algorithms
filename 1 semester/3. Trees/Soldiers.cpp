/*
В одной военной части решили построить в одну шеренгу по росту. 
Т.к. часть была далеко не образцовая, то солдаты часто приходили не вовремя, 
а то их и вовсе приходилось выгонять из шеренги за плохо начищенные сапоги. 
Однако солдаты в процессе прихода и ухода должны были всегда быть выстроены 
по росту – сначала самые высокие, а в конце – самые низкие. 
За расстановку солдат отвечал прапорщик, который заметил интересную особенность –
все солдаты в части разного роста. 
Ваша задача состоит в том, чтобы помочь прапорщику правильно расставлять солдат, 
а именно для каждого приходящего солдата указывать, перед каким солдатом в строе 
он должен становится. 
Требуемая скорость выполнения команды - O(log n).

Формат ввода
Первая строка содержит число N – количество команд (1 ≤ N ≤ 90 000). 
В каждой следующей строке содержится описание команды: число 1 и X если 
солдат приходит в строй (X – рост солдата, натуральное число до 100 000 
включительно) и число 2 и Y если солдата, стоящим в строе на месте Y надо удалить из строя. 
Солдаты в строе нумеруются с нуля.

Формат вывода
На каждую команду 1 (добавление в строй) вы должны выводить число K – номер позиции, 
на которую должен встать этот солдат (все стоящие за ним двигаются назад).
*/
#include <iostream>

template <typename T>
class Node {
public:
    T key;              //рост солдата
    int height;         //высота узла
    int under_sum;      //кол-во узлов, расположенных под текущим, включая его
    Node* left;
    Node* right;
    explicit Node(T key):
        key(key),
        height(1),
        under_sum(1),
        left(nullptr),
        right(nullptr){}
};

template <typename M>
class AVL{
//    корень
    Node <M>* root;
//    возвращает кол-во узлов (далее "сумма") под узлом, включая его самого
    int sum(Node <M>* &head){
        return head == nullptr ? 0 : head->under_sum;
    }
//    возвращает высоту узла
    int height(Node <M>* &head){
        return head == nullptr ? 0 : head->height;
    }
//    восстанавливает корректность высоты узла
    void fix_height(Node <M>* &head){
//        балансировка суммы
        head->under_sum = sum(head->left) + sum(head->right) + 1;
//        балансировка высоты
        head->height = (height(head->left) < height(head->right) ? height(head->right) : height(head->left)) + 1;
    }
//    возвращает разницу между высотами правого и левого узлов
    int difference(Node <M>* &head){
        return height(head->right) - height(head->left);
    }
//    малое левое вращение
    Node <M>* left_rotate(Node <M>* &head){
        Node <M>* tmp = head->right;
        head->right = tmp->left;
        tmp->left = head;
        fix_height(head);
        fix_height(tmp);
        return tmp;
    }
//    малое правое вращение
    Node <M>* right_rotate(Node <M>* &head){
        Node <M>* tmp = head->left;
        head->left = tmp->right;
        tmp->right = head;
        fix_height(head);
        fix_height(tmp);
        return tmp;
    }
//    балансировка дерева
    Node <M>* balance(Node <M>* &head){
        fix_height(head);
//        большое левое вращение
        if (difference(head) == 2){
            if (difference(head->right) < 0)
                head->right = right_rotate(head->right);
            return left_rotate(head);
        }
//        большое правое вращение
        if (difference(head) == -2){
            if (difference(head->left) > 0)
                head->left = left_rotate(head->left);
            return right_rotate(head);
        }
        return head;
    }
//    поиск минимального узла (используется в удалении); возвращает указатель на минимальный узел
    Node <M>* find_min(Node <M>* &head){
        if (head->left == nullptr){
            return head;
        }
        return find_min(head->left);
    }
//    меняет левый указатель родителя минимального узла на правое поддерево минимального узла
//    восстанавливает баланс изменёного дерева
    Node <M>* move_min(Node <M>* &head){
        if (head->left == nullptr){
            return head->right;
        }
        head->left = move_min(head->left);
        return balance(head);
    }
//    вставка элемента к (вспомогательная функция)
    Node <M>* insert(Node <M>* &current, const int &k, int &position) {
//    вставляем элемент в листовую вершину
        if (current == nullptr){
            std::cout<<position<<'\n';
            return new Node <M> (k);
        }
//    спускаемся по ключам
        if (k < current->key){
            position += sum(current->right) + 1;
            current->left = insert(current->left, k, position);
        } else{
            current->right = insert(current->right, k, position);
        }
//    восстанавливаем баланс дерева
        return balance(current);
    }
//    удаление узла по ключу k (вспомогательная функция)
    Node <M>* remove(Node <M>* &current, const int &position, int &current_pos) {
        //    спускаемся по дереву в поисках удаляемого элемента
        if (position > current_pos + sum(current->right)){
            current_pos += sum(current->right) + 1;
            current->left = remove(current->left, position, current_pos);
        } else{
            if (position < current_pos + sum(current->right)){
                current->right = remove(current->right, position, current_pos);
            } else{
//            нашли нужный узел
                if (position == current_pos + sum(current->right)){
//        если у удаляемого (текущего) узла нет правого поддерева, то удаляем текущий,
//        возвращаем его левое поддерево
                    if (current->right == nullptr){
                        Node <M>* tmp = current->left;
                        delete current;
                        return tmp;
//        если же у удаляемого узла есть правое поддерево, то находим минимальный узел в правом
//        поддереве и ставим его на место текущего (переводя указатель родителя извлечённого
//        узла на правое поддерево, если оно есть)
                    } else{
                        Node <M>* tmp_left = current->left;
                        Node <M>* tmp_right = current->right;
                        delete current;
//            ищем минимальный в правом поддереве
                        Node <M>* min = find_min(tmp_right);
                        min->right = move_min(tmp_right);
                        min->left = tmp_left;
                        return balance(min);
                    }
                }
            }
        }
//    восстанавливаем баланс дерева
        return balance(current);
    }
//    удаление узла (используется в деструкторе)
    void destruct(Node <M>* current){
        if (current == nullptr)
            return;
        destruct(current->left);
        destruct(current->right);
        delete current;
    }
public:
    explicit AVL(): root(nullptr) {}
//    вставка элемента k
    void insertion(const int &k){
        int position = 0;
        root = insert(root, k, position);
    }
//    удаление узла по ключу k
    void removal(const int &position){
        int current_pos = 0;
        root = remove(root, position, current_pos);
    }
    ~AVL(){
        destruct(root);
    }
};

int main() {
    AVL <int> soldiers;
    int N, command, X;
    std::cin>>N;
    for (int i = 0; i < N; ++i){
        std::cin>>command>>X;
        command == 1 ? soldiers.insertion(X) : soldiers.removal(X);
    }
    return 0;
}
