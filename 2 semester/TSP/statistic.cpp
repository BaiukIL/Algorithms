#include "tsp_problem/exact_tsp_solution.h"
#include "tsp_problem/double_tsp_solution.h"
#include "tsp_problem/christofides_tsp_solution.h"
#include <random>

Graph GenerateMap(uint graphVerticesNumber) {
  std::random_device rd;
  std::mt19937 generator(rd());
  std::normal_distribution<> distribution{5, 10};

  std::vector<std::pair<uint, uint>> dots;
  for (int i = 0; i < graphVerticesNumber; ++i) {
    uint x = std::round(distribution(generator));
    uint y = std::round(distribution(generator));
    dots.emplace_back(std::make_pair(x, y));
  }

  Graph map(graphVerticesNumber);
  for (int i = 0; i < graphVerticesNumber - 1; ++i) {
    for (int j = i + 1; j < graphVerticesNumber; ++j) {
      int dx = dots[i].first - dots[j].first;
      int dy = dots[i].second - dots[j].second;
      weight_t weight = std::sqrt(dx*dx + dy*dy);
      map.AddEdge(i, j, weight);
    }
  }
  return map;
}

void GetStatistic(weight_t exact, weight_t approx, std::pair<weight_t, double> &statistic) {
//  std::pair<weight_t, double> ~ (total weight, total sum of squares of differences)
  statistic.first += approx;
  weight_t diff = approx - exact;
  statistic.second += diff * diff;
}

void GetFullStatistic(uint maxVerticesNum, uint repeatNum,
                          std::vector<std::pair<weight_t, double>> &double_statistic,
                          std::vector<std::pair<weight_t, double>> &christ_statistic) {
  double_statistic.resize(maxVerticesNum + 1);
  christ_statistic.resize(maxVerticesNum + 1);

  for (int N = 2; N <= maxVerticesNum; ++N) {
    for (int i = 1; i <= repeatNum; ++i) {
      Graph map = GenerateMap(N);
      weight_t exactPathWeight = ExactTravelPathWeight(map);
      GetStatistic(exactPathWeight, DoubleTravelPathWeight(map), double_statistic[N]);
      GetStatistic(exactPathWeight, ChristofidesTravelPathWeight(map), christ_statistic[N]);
    }
//    average weights over N
    double_statistic[N].first /= repeatNum;
    christ_statistic[N].first /= repeatNum;
//    count r.m.s deviations
    double_statistic[N].second = std::sqrt(double_statistic[N].second / repeatNum);
    christ_statistic[N].second = std::sqrt(christ_statistic[N].second / repeatNum);
  }
}

void PrintStatistic(uint N, std::vector<std::pair<weight_t, double>> &statistic) {
  std::cout << "\t\t\t" << std::round(statistic[N].first)
            << "\t\t\t\t\t\t\t" << std::round(statistic[N].second)
            << "\t\t\t\t\t\t\t\t" << std::round(statistic[N].second / statistic[N].first * 100);
}

int main() {
  uint maxVerticesNum = 10;
  uint repeatNum = 10;
  std::vector<std::pair<weight_t, double>> double_statistic;
  std::vector<std::pair<weight_t, double>> christ_statistic;
  GetFullStatistic(maxVerticesNum, repeatNum, double_statistic, christ_statistic);

  std::cout << "Vertices number\t\tAverage double weight\t\tR.M.S. double deviations\t\tin %\t\t"
               "Average christ. weight\t\tR.M.S. christ. deviations\t\tin %\n";

  for (int N = 2; N <= maxVerticesNum; ++N) {
    std::cout << N << "\t\t";
    PrintStatistic(N, double_statistic);
    PrintStatistic(N, christ_statistic);
    std::cout << '\n';
  }
}
