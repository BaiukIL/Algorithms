#include <iostream>
#include <vector>
#include <queue>
#include "Huffman.h"

class InBitStream
{
    std::vector <byte> buffer;          //битовый буфер
    int count_bit = 0;                  //счётчик записанных в текущий последний элемент буфера битов
    int count_size = 0;                 //счётчик итераций для считывания
public:
    void push_bit(bool bit);            //записывает бит в конец буфера
    void push_byte(byte symbol);        //записывает символ в конец буфера в двоичном представлении
//    void print();                       //для отладки
    void add_empty_bits();              //вставляет в начало буфера количество незаполненных бит в последнем байте буфера
    bool read(byte &value);             //считывает байты из буфера
};
void InBitStream::push_bit(bool bit) {
    if (buffer.empty()){
        buffer.push_back(0);
        count_bit = 3;
    }
    if (count_bit == 0){
        buffer.push_back(0);
    }
    buffer.back() |= bit << count_bit;
    count_bit = (count_bit + 1) % 8;
}
void InBitStream::push_byte(byte symbol) {
    for (int i = 0; i < 8; ++i){
        push_bit(static_cast<bool>((symbol >> i) & 1));
    }
}
//void InBitStream::print() {
//    for (byte symbol : buffer) {
//        count_bit = 0;
//        while (count_bit != 8){
//            std::cout<< ((symbol >> count_bit) & 1);
//            ++count_bit;
//        }
//    }
//}
void InBitStream::add_empty_bits() {
    buffer[0] |= count_bit;
}
bool InBitStream::read(byte &value) {
    if (count_size == buffer.size())
        return false;
    value = buffer[count_size];
    ++count_size;
    return true;
}
struct Pair
{
    byte symbol;
    std::vector <short> code;
    Pair(byte symbol, std::vector <short> code): symbol(symbol), code(std::move(code)){}
};
struct Node;
bool cmp (const Node* a, const Node* b);
struct Node
{
    byte symbol;
    int weight;
    Node* left;
    Node* right;
    explicit Node(const byte symbol, const int weight):
            symbol(symbol),
            weight(weight),
            left(nullptr),
            right(nullptr) {}
//        создаём пустой "0" узел, цепляем слева и справа
    Node* merge(Node* other){
        Node* parent = new Node ('0', weight + other->weight);
        if (cmp(this, other)){
            parent->left = other;
            parent->right = this;
        } else{
            parent->left = this;
            parent->right = other;
        }
        return parent;
    }
    void Build_tree(InBitStream &a){
        visit(a, this);
    }
    void Build_alphabet(std::vector <Pair> &Huffman_alphabet){
        std::vector <short> bits;
        alphabet_visit(this, bits, Huffman_alphabet);
    }
    void alphabet_visit(Node* current, std::vector <short> &bits, std::vector <Pair> &Huffman_alphabet){
        if (current->left == nullptr && current->right == nullptr){
            Pair tmp(current->symbol, bits);
            Huffman_alphabet.push_back(tmp);
        } else{
            bits.push_back(0);
            alphabet_visit(current->left, bits, Huffman_alphabet);
            bits.pop_back();
            bits.push_back(1);
            alphabet_visit(current->right, bits, Huffman_alphabet);
            bits.pop_back();
        }
    }
    ~Node(){
        remove(this);
    }
private:
    void visit(InBitStream &a, Node* current){
        if (current->left == nullptr && current->right == nullptr){
            a.push_bit(true);
            a.push_byte(current->symbol);
        } else{
            a.push_bit(false);
            visit(a, current->left);
            visit(a, current->right);
        }
    }
    void remove(Node* current){
        if (current == nullptr){
            return;
        }
        remove(current->left);
        remove(current->right);
        delete current;
    }
};
struct Compare
{
    bool operator()(const Node* a, const Node* b){
        return cmp(a, b);
    }
};
bool cmp(const Node* a, const Node* b){
    return a->weight >= b->weight;
}
void InCopyStream(IInputStream &original, InBitStream &bits){
    std::vector <Node*> alphabet;
    alphabet.reserve(256);
    for (int i = 0; i < 256; ++i){
        alphabet.push_back(new Node((byte)i, 0));
    }
    byte symbol;
    std::vector <byte> tmp_buffer;
    while (original.Read(symbol)){
        ++alphabet[symbol]->weight;
        tmp_buffer.push_back(symbol);
    }
    std::priority_queue <Node*, std::vector<Node*>, Compare> buffer;
    for (int i = 0; i < 256; ++i){
        if (alphabet[i]->weight != 0)
            buffer.push(alphabet[i]);
    }
    alphabet.clear();
    Node* tree;
    while (buffer.size() != 1){
        Node* a = buffer.top();
        buffer.pop();
        Node* b = buffer.top();
        buffer.pop();
        tree = a->merge(b);
        buffer.push(tree);
    }
    while(!buffer.empty()){
        buffer.pop();
    }
    tree->Build_tree(bits);
    std::vector <Pair> Huffman_alphabet;
    tree->Build_alphabet(Huffman_alphabet);
    for (byte value : tmp_buffer){
        int i = 0;
        while (value != Huffman_alphabet[i].symbol){   //ищем символ в алфавите
            ++i;
        }
        for (bool j : Huffman_alphabet[i].code) {
            bits.push_bit(j);
        }
    }
}
void Encode(IInputStream &original, IOutputStream &compressed){
    InBitStream bits;
    InCopyStream(original, bits);
    bits.add_empty_bits();
    byte symbol;
    while(bits.read(symbol)){
        compressed.Write(symbol);
    }
}

class OutBitStream
{
    std::vector <byte> buffer;          //битовый буфер
    int count_bit = 0;                  //счётчик записанных в текущий последний элемент буфера битов
    int count_size = 0;                 //счётчик итераций для считывания
    int count_step = -1;
//первые три бита - количество записанных в последний байт битов
    int out_number = 0;
public:
    void push_bit(bool bit);            //записывает бит в конец буфера
    void push_byte(byte symbol);        //записывает символ в конец буфера в двоичном представлении
//    void print();                       //для отладки
    bool top_bit();                     //получение бита
    void top_byte(byte &symbol);
    bool empty();
    long size();
    void set_out_number();
};
void OutBitStream::push_bit(bool bit) {
    if (count_bit == 0){
        buffer.push_back(0);
    }
    buffer.back() |= bit << count_bit;
    count_bit = (count_bit + 1) % 8;
}
void OutBitStream::push_byte(byte symbol) {
    for (int i = 0; i < 8; ++i){
        push_bit(static_cast<bool>((symbol >> i) & 1));
    }
}
//void OutBitStream::print() {
//    while(count_step != buffer.size() - 1) {
//        for (int i = 0; i < 8; ++i){
//            bool tmp = top_bit();
//            std::cout<<tmp;
//        }
//    }
//}
bool OutBitStream::top_bit() {
    if (count_bit == 0){
        ++count_step;
    }
    bool tmp = static_cast<bool>((buffer[count_step] >> count_bit) & 1);
    count_bit = (count_bit + 1) % 8;
    return tmp;
}
void OutBitStream::top_byte(byte &symbol) {
    for (int i = 0; i < 8; ++i){
        symbol |= top_bit() << i;
    }
}
bool OutBitStream::empty() {
    return count_step * 8 + count_bit == buffer.size() * 8 - 8 + out_number;
}
void OutBitStream::set_out_number() {
    for (int i = 0; i < 3; ++i){
        out_number |= top_bit() << i;
    }
}
long OutBitStream::size() {
    return buffer.size();
}
struct DecodeTreeNode
{
    byte symbol;
    DecodeTreeNode* left;
    DecodeTreeNode* right;
    explicit DecodeTreeNode(const byte symbol):
            symbol(symbol),
            left(nullptr),
            right(nullptr){}
};
class DecodeTree
{
    DecodeTreeNode* root;
    void remove(DecodeTreeNode* current){
        if (current == nullptr){
            return;
        }
        remove(current->left);
        remove(current->right);
        delete current;
    }
    byte find(OutBitStream &bits, DecodeTreeNode* current){
        if (current->left == nullptr && current->right == nullptr){
            return current->symbol;
        }
        bool tmp = bits.top_bit();
        tmp ? find(bits, current->right) : find(bits, current->left);
    }
    DecodeTreeNode* build(OutBitStream &bits, DecodeTreeNode* current){
        bool tmp = bits.top_bit();
        if (tmp){
            byte symbol = 0;
            bits.top_byte(symbol);
            return new DecodeTreeNode(symbol);
        } else{
//            фиктивная вершина
            current = new DecodeTreeNode('0');
            current->left = build(bits, current->left);
            current->right = build(bits, current->right);
        }
    }
public:
    DecodeTree() {root = nullptr;}
    void build_tree(OutBitStream &bits){
        root = build(bits, root);
    }
    byte next_symbol(OutBitStream &bits){
        return find(bits, root);
    };
    ~DecodeTree(){
        remove(root);
    }
};
void OutCopyStream(IInputStream &compressed, OutBitStream &bits){
    byte symbol;
    while (compressed.Read(symbol)){
        bits.push_byte(symbol);
    }
}
void Decode(IInputStream &compressed, IOutputStream &original){
    OutBitStream bits;
    OutCopyStream(compressed, bits);
    if (bits.size() == 0){
        return;
    }
    bits.set_out_number();
    DecodeTree Huffman_alphabet;
    Huffman_alphabet.build_tree(bits);
    while (!bits.empty()){
        original.Write(Huffman_alphabet.next_symbol(bits));
    }
}
int main() {
    //Мой main
    	Reader original("path_to_Initial");
    	Writer compressed("path_to_Coded");
	Encode(original, compressed);
	Reader encoded("path_to_Coded");
	Writer decoded("path_to_Decoded");
	Decode(encoded, decoded);
    return 0;
}
