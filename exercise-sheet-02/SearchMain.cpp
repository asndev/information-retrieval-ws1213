// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include "./QueryProcessor.h"
#include "./InvertedIndex.h"

using std::string;

// Main function.
int main(int argc, char** argv) {
  // Parse command line arguments.
  if (argc < 4 || argc > 7) {
    printf("Usage: SearchMain <name of collection CSV file> <keyword1>"
           " <keyword2> <number of result records> <BM25k> <BM25b>"
           "\n");
    exit(1);
  }
  string word1 = (string)argv[2];
  string word2 = (string)argv[3];
  string query = word1 + " " + word2;
  // set standard k, if none is set.
  int k = 3;
  float bm25k = 1.75f, bm25b = 0.75f;
  if (argc >= 5) { k = atoi(argv[4]); }
  if (argc >= 6) { bm25k = atof(argv[5]); }
  if (argc >= 7) { bm25b = atof(argv[6]); }
  InvertedIndex ii;
  ii.buildFromCsvFile(argv[1], bm25k, bm25b);
  QueryProcessor qp(ii);
  vector<string> result = qp.answerQuery(query, k);

  // print the result strings in a readable way
  if (result.size() == 0) {
    std::cout << "no matches found." << std::endl;
    exit(1);
  }

  for (vector<string>::iterator it = result.begin();
       it != result.end(); ++it) {
    qp.prettyPrint(*it, word1, word2);  // Print with highlights
    // qp.print(*it);
  }
}
