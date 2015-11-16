// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>

#include <chrono>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>
#include "./Intersect.h"

// #calculations with each algorithm.
const size_t numberOfRuns = 5;
const int totalNumberOfElements = 250000000;

// Main function.
int main(int argc, char** argv) {
  using std::cout;
  using std::string;
  using std::vector;
  // Parse command line arguments.
  if (argc < 2) {
    cout << argv[0] << " <ratio R>" << std::endl;
    return 1;
  }
  int R = atoi(argv[1]);
  // Set the size of the two lists
  const int list1Size = totalNumberOfElements/(R+1);
  const int list2Size = totalNumberOfElements - list1Size;
  // Generate two list of size n and n*r.
  vector<int> list1, list2;
  std::cout << "Genereting Lists of appr. size " << list1Size/1e6 << "M and "
            << list2Size/1e6 << "M ..." << std::flush;
  auto start = std::chrono::high_resolution_clock::now();
  // generating random numbers for the lists.
  int biggerList = std::max(list1Size, list2Size);
  unsigned int seed = (unsigned int)time(NULL);
  int universe = biggerList * 5;
  // list creation
  double listProb1 = static_cast<double>(list1Size) /
                    static_cast<double>(universe);
  double listProb2 = static_cast<double>(list2Size) /
                    static_cast<double>(universe);
  double f;
  for (int i = 0; i < universe; ++i) {
    f = (static_cast<double>(rand_r(&seed)) / (RAND_MAX));
    if (f <= listProb1) list1.push_back(i);
    if (f <= listProb2) list2.push_back(i);
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::cout << "done in "
  << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
  << "ms" << std::endl << std::endl;

  /*int l1size = list1.size();
  for (int i = 1; i < l1size; ++i) {
    assert(list1[i-1] < list1[i]);
  }
  int l2size = list2.size();
  for (int j = 1; j < l2size; ++j) {
    assert(list2[j-1] < list2[j]);
  }*/
  std::cout << "Size of lists: 1)" << list1.size() << " 2) "
            << list2.size() << std::endl;
  // Intersection Algorithm V1
  for (size_t i = 0; i < numberOfRuns; ++i) {
    std::cout << "Using IntersectV1 ..." << std::flush;
    start = std::chrono::high_resolution_clock::now();
    vector<int> result = Intersect::IntersectV1(list1, list2);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "done in "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
    << "ms" << std::endl;
    assert(result.size() <= std::min(list1.size(), list2.size()));
  }
  std::cout << std::endl;
  // Intersection Algorithm V2
  for (size_t i = 0; i < numberOfRuns; ++i) {
    std::cout << "Using IntersectV2 ..." << std::flush;
    start = std::chrono::high_resolution_clock::now();
    vector<int> result = Intersect::IntersectV2(list1, list2);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "done in "
    << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count()
    << "ms" << std::endl;
    assert(result.size() <= std::min(list1.size(), list2.size()));
  }
}
