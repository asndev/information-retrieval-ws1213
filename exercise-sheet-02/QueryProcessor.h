// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#ifndef EXERCISE_SHEET_02_QUERYPROCESSOR_H_
#define EXERCISE_SHEET_02_QUERYPROCESSOR_H_

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>
#include "./InvertedIndex.h"

// Class for processing queries with two keywords based on an inverted index.
class QueryProcessor {
 public:
  // constructor to use with a reference to a given inverted index
  explicit QueryProcessor(const InvertedIndex& ii);

  // Return vector with urls and records of size k for a given query.
  std::vector<std::string> answerQuery(const std::string& query, const int k);

  // Intersect two inverted lists and return the result list.
  std::vector<Posting> intersect(const std::vector<Posting>& list1,
                                 const std::vector<Posting>& list2);
  FRIEND_TEST(IntersectTest, intersect);

  // Prettyprint the result with bold highlight words.
  void prettyPrint(std::string resultLine, std::string word1,
                   std::string word2);
  void print(std::string resultLine);

 private:
  // The inverted index.
  const InvertedIndex& _index;
};

#endif  // EXERCISE_SHEET_02_QUERYPROCESSOR_H_
