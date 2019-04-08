//Задано N точек на плоскости. 
//Указать (N-1)-звенную несамопересекающуюся замкнутую ломаную, проходящую через все эти точки.
//Для сортировки точек реализуйте алгоритм сортировки вставками.
//В первой строке записано единственное число — число точек N. N ≤ 10000. 
//В последующих N строках записаны координаты точек xi, yi. 
//Все координаты целочисленные xi, yi ≤ 100000.

#include <iostream>
 
class Point
{
    double tan;
public:
    int x;
    int y;
    bool bigger(const Point prev)
    {
        return tan > prev.tan;
    }
    void count_tan(const Point min)
    {
        int dif_x, dif_y;
        dif_x = x - min.x;
        dif_y = y - min.y;
        if (dif_x != 0)
            tan = ((double)dif_y / dif_x);
        else
            if (dif_y != 0)
                tan = 100001;  //точка, имеющая одинаковый х с минимальной (выводится второй)
            else
                tan = 100002;  //сама минимальная точка (выводится первой)
    }
};
 
void Find_min(const Point polygon, Point &min, const int itter)
{
    if (itter == 0)
    {
        min.x = polygon.x;
        min.y = polygon.y;
    }
    else
        if (polygon.x <= min.x)
        {
            if (polygon.x == min.x)
            {
                if (polygon.y < min.y)
                    min.y = polygon.y;
            }
            else
            {
                min.x = polygon.x;
                min.y = polygon.y;
            }
        }
}
 
void InsertSort(Point* arr, const Point min, const int size)
{
    for (int i = 0; i < size; ++i)
    {
        arr[i].count_tan(min);
        Point current = arr[i];
        int j = i;
        for (; j > 0 && current.bigger(arr[j-1]); --j)
        {
            arr[j] = arr[j-1];
        }
        arr[j] = current;
    }
}
void print(const Point* polygon, const int size)
{
    for (int i = 0; i < size; ++i)
    {
        std::cout<<polygon[i].x<<' '<<polygon[i].y<<'\n';
    }
}
int main()
{
    int n;
    std::cin>>n;
    Point* polygon = new Point [n];
    Point min;
    for (int i=0; i<n; ++i)
    {
        std::cin>>polygon[i].x>>polygon[i].y;
        Find_min(polygon[i], min, i);	//находим минимальные координаты x, y
    }
 
    InsertSort(polygon, min, n);
 
    print(polygon, n);
 
    delete [] polygon;
    return 0;
}
