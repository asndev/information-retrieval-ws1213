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
using std::map;
using std::vector;


// ____________________________________________________________________________
TEST(QueryProcessorTest, intersect) {
  InvertedIndex ii;
  ii.buildFromCsvFile("./simple.csv", 1.75f, 0.75f);

  QueryProcessor qp(ii);

  vector<Posting> list1;
  vector<Posting> list2;
  Posting p1(1, 1.0f);
  Posting p2(4, 0.5f);
  Posting p3(7, 0.75f);
  list1.push_back(p1);
  list1.push_back(p2);
  list1.push_back(p3);
  Posting p4(2, 1.8f);
  Posting p5(4, 1.3f);
  Posting p6(5, 0.7f);
  Posting p7(7, 2.25f);
  list2.push_back(p4);
  list2.push_back(p5);
  list2.push_back(p6);
  list2.push_back(p7);
  vector<Posting> v = qp.intersect(list1, list2);
  ASSERT_EQ(2, v.size());
  ASSERT_EQ(1.8f, v[0].score);
  ASSERT_EQ(3.0f, v[1].score);
}
