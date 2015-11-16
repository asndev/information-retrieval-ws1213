// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "./InvertedIndex.h"
#include "./Posting.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
using std::endl;
using std::flush;

const float BM25_B = 0.75;
const float BM25_K = 1.75;

int main(int argc, char** argv) {
  // Parse command line arguments.
  if (argc != 4) {
    std::cout << "Usage: " << argv[0] << " <inputfile> <outputfile> <m>"
              << std::endl;
    return 1;
  }
  InvertedIndex ii;
  cout << "Building InvertedIndex ii ..." << flush;
  auto start = high_resolution_clock::now();
  ii.buildFromCsvFile(argv[1], BM25_K, BM25_B);
  auto end = high_resolution_clock::now();
  cout << "done in " << duration_cast<milliseconds>(end-start).count() << "ms."
       << endl;
  cout << "Writing TDMatrix and Terms ..." << flush;
  start = high_resolution_clock::now();
  ii.WriteTermDocumentMatrix(argv[2], atoi(argv[3]));
  end = high_resolution_clock::now();
  cout << "done in " << duration_cast<milliseconds>(end-start).count() << "ms."
       << endl;
}
