// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#ifndef EXERCISE_SHEET_01_QUERYPROCESSOR_H_
#define EXERCISE_SHEET_01_QUERYPROCESSOR_H_

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>
#include "./InvertedIndex.h"

using std::ifstream;
using std::map;
using std::string;
using std::vector;

// Class for processing queries with two keywords based on an inverted index.
class QueryProcessor {
 public:
  // empty constructor used by test cases
  QueryProcessor();
  // constructor to use with a reference to a given inverted index
  explicit QueryProcessor(const InvertedIndex& ii);

  // Answer single query. Return list of matching record ids.
  vector<int> answerSingleQuery(string word);

  // Return vector with urls and records of size k for a given query.
  vector<string> answerQuery(string query, int k);

  // Intersect two inverted lists and return the result list.
  vector<int> intersect(vector<int> list1, vector<int> list2);
  FRIEND_TEST(IntersectTest, intersect);

  // Prettyprint the result with bold highlight words.
  void prettyPrint(string resultLine, string word1, string word2);
  void print(string resultLine);

 private:
  // The inverted index.
  InvertedIndex _index;
};

#endif  // EXERCISE_SHEET_01_QUERYPROCESSOR_H_
