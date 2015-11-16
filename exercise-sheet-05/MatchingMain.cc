// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>

#include <chrono>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "./ApproximateMatching.h"

// Main function.
int main(int argc, char** argv) {
  using std::string;
  // Parse commandline arguments
  if (argc != 3) {
    std::cout << "Usage: " << argv[0] << " <vocabular> <queries>" << std::endl;
    return 1;
  }
  string vocabular = (string) argv[1];
  string queries = (string)argv[2];
  size_t k = 3;
  ApproximateMatching am;
  std::cout << "Constructing index..." << std::flush;
  auto start = std::chrono::high_resolution_clock::now();
  am.BuildIndex(vocabular, k);
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "done in \033[1m\033[31m"
  << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
  << "ms\033[0m." << std::endl << std::endl;
  // Iterate over queries/
  std::ifstream data(queries.c_str());
  string query;
  size_t matches = 0;
  float avgQueryTime = 0.0f;
  std::cout << "Processing queries..." << std::endl;
  while (true) {
    getline(data, query);
    if (data.eof()) break;
    auto start = std::chrono::high_resolution_clock::now();
    matches += am.ComputeApproximateMatches(query).size();
    auto end = std::chrono::high_resolution_clock::now();
    avgQueryTime +=
    std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
  }
  std::cout << "Average Querytime: \033[1m\033[31m" << avgQueryTime/1000.0
            << "ms\033[0m." << std::endl;
  std::cout << "Average Number of matches: \033[1m\033[31m" << matches/1000.0
            << "\033[0m." << std::endl;
  std::cout << "Average EditDistance calculation time: \033[1m\033[31m"
          << am.AvgEdTimeInNano/(1000*(static_cast<double>(am.EdComputations)))
            << "micros\033[0m."
            << std::endl;
}
