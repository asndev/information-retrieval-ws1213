// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.

#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include "./InvertedIndex.h"
#include "./ApproximateMatching.h"
#include "./SearchServer.h"

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::cout;
using std::endl;
using std::flush;

const float BM25_B = 0.75;
const float BM25_K = 1.75;
const size_t KGRAMS_K = 3;

int main(int argc, char** argv) {
  // Parse command line arguments.
  if (argc != 3) {
    std::cout << "Usage: ./SearchServer <vocabulary file> <port>" << std::endl;
    exit(1);
  }
  std::string vocabularyFileName = argv[1];
  size_t port = atoi(argv[2]);
  ApproximateMatching am;
  InvertedIndex ii;
  cout << "Building InvertedIndex ii ..." << flush;
  auto start = high_resolution_clock::now();
  ii.buildFromCsvFile(vocabularyFileName, BM25_K, BM25_B);
  auto end = high_resolution_clock::now();
  cout << "done in " << duration_cast<milliseconds>(end-start).count() << "ms."
       << endl;
  cout << "Getting all words ..." << flush;
  start = high_resolution_clock::now();
  std::vector<std::string> words = ii.getAllWords();
  end = high_resolution_clock::now();
  cout << "done in " << duration_cast<milliseconds>(end-start).count() << "ms."
       << endl;
  cout << "Builing AM Index ..." << flush;
  start = high_resolution_clock::now();
  am.BuildIndex(words, KGRAMS_K);
  end = high_resolution_clock::now();
  cout << "done in " << duration_cast<milliseconds>(end-start).count() << "ms."
       << endl;
  SearchServer Server(am, ii, port);
  Server.ServerLoop();
}
