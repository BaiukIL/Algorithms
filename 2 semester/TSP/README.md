Travelling salesman problem.

There are two solutions:
1. Double approximaion. Build MSP (minimum spanning tree) and use DFS (depth first search) bypass to find approximation.
2. Christofides algorithm. This one gives 3/2 worst-case approximation. However, it works as long as exact algorithm because of straightforward minimum weigth perfect matching search. One can implement it in O(n^3), but it is much more difficult problem.

`statistic.cpp` shows statistic on TSP solutions: 2-opt(double) and Christofides.

