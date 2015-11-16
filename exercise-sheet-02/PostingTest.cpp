// Copyright 2012, University of Freiburg
// Anton Stepan, Student of Computer Science
// <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>
#include "./Posting.h"

using std::string;
using std::vector;

bool compareFunction(Posting i, Posting j) { return (i.score > j.score); }

// ____________________________________________________________________________
TEST(Posting, operators) {
  Posting p1(1, 0.9f);
  Posting p2(1, 0.9f);
  Posting p3(2, 0.5f);
  Posting p4(1, 0.2f);

  ASSERT_EQ(1, p1 == p2);
  ASSERT_EQ(0, p1 == p3);
  ASSERT_EQ(0, p1 == p4);
  ASSERT_EQ(0, p4 == p3);

  Posting p5(4, 1.4f);
  Posting p6(5, 1.4f);

  vector<Posting> v;
  v.push_back(p1);
  v.push_back(p2);
  v.push_back(p3);
  v.push_back(p4);
  v.push_back(p5);
  v.push_back(p6);

  std::partial_sort(v.begin(), v.begin() + 3, v.end(), compareFunction);
  ASSERT_EQ(1.4f, v[0].score);
  ASSERT_EQ(1.4f, v[1].score);
  ASSERT_EQ(0.9f, v[2].score);
}
