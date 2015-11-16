// Copyright 2012, University of Freiburg
// Anton Stepan, Student of Computer Science
// <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "./SearchServer.h"
#include "./ApproximateMatching.h"
#include "./InvertedIndex.h"

using std::string;
using std::vector;
using std::map;

// ____________________________________________________________________________
TEST(SearchServerTest, constructor) {
  InvertedIndex ii;
  ApproximateMatching am;
  SearchServer sv(am, ii, 8888);
  ASSERT_EQ(8888, sv._port);
}
