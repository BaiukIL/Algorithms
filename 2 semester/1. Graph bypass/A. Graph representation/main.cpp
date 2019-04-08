#include <iostream>
#include "list_graph.h"
#include "matrix_graph.h"
#include "set_graph.h"
#include "arc_graph.h"

int main() {
  IGraph* graphList = new ListGraph(5);

  graphList->AddEdge(0, 2);
  graphList->AddEdge(2, 4);
  graphList->AddEdge(4, 0);
  graphList->AddEdge(1, 3);
  graphList->AddEdge(3, 1);

  IGraph* graphArc = new ArcGraph(graphList);
  IGraph* setGraph = new SetGraph(graphArc);
  IGraph* matrixGraph = new MatrixGraph(setGraph);

  std::vector<int> tmp;
  for (int i = 0; i < graphArc->VerticesCount(); ++i) {
    graphArc->GetNextVertices(i, tmp);
  }

  std::cout << "Yes\n";

  delete graphList;
  delete graphArc;
  delete setGraph;
  delete matrixGraph;

  return 0;
}







//class Shape {
//public:
//    virtual void area () const = 0;
//    virtual ~ Shape() = default;
//};
//class Tetragon : public Shape {};
//class Rectangle : public Tetragon {
//protected:
//    int width;
//    int height;
//public:
//    explicit Rectangle (int width, int height) : width(width), height(height) {}
//    virtual void area () const override { std::cout << "Rectangle: " << width * height <<'\n'; }
//    virtual ~ Rectangle() = default;
//};
//class Square final : public Rectangle {
//public:
//    Square (int side) : Rectangle(side, side) {}
//    void area () const override { std::cout << "Square: " << width * width << '\n'; }
//};
//class Triangle : public Shape {
//private:
//    int P;
//protected:
////    sides
//    int a, b, c;
//public:
//    explicit Triangle (int a, int b, int c) : a(a), b(b), c(c), P(a+b+c) {}
//    virtual void area () const {
//      std::cout << "Triangle: " << a * b << '\n';
//    }
//};