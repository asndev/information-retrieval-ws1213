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
  if (argc != 4 && argc != 5) {
    printf("Usage: SearchMain <name of collection CSV file> <keyword1>"
           " <keyword2> <number of result records>"
           "\n");
    exit(1);
  }
  string word1 = (string)argv[2];
  string word2 = (string)argv[3];
  string query = word1 + " " + word2;
  // set standard k, if none is set.
  int k = 3;
  if (argc == 5) { k = atoi(argv[4]); }
  InvertedIndex ii;
  ii.buildFromCsvFile(argv[1]);
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
