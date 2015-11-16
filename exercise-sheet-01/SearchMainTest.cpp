// Copyright 2012, University of Freiburg
// Anton Stepan, Student of Computer Science
// <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>
#include "./QueryProcessor.h"
#include "./InvertedIndex.h"

using std::string;

// ____________________________________________________________________________
TEST(SearchMainTest, Main) {
  string query = "nice website";
  InvertedIndex ii;
  ii.buildFromCsvFile("./simple.csv");

  QueryProcessor qp(ii);
  vector<string> result = qp.answerQuery(query, 3);
  ASSERT_EQ(1, result.size());
  query = "nice nice";
  result = qp.answerQuery(query, 3);
  ASSERT_EQ(2, result.size());
}
