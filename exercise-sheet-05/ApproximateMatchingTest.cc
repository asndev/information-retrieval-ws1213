// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>
#include "./ApproximateMatching.h"

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, BuildIndex) {
  ApproximateMatching am;
  am.BuildIndex("./simple.txt", 3);
  ASSERT_EQ(3, am._k);
  ASSERT_EQ(45, am._words.size());
  ASSERT_EQ(10, am._invertedLists["aap"].size());
  ASSERT_EQ(0, am._invertedLists["xxx"].size());
  ASSERT_EQ(44, am._invertedLists["#aa"].size());
  ASSERT_EQ(1, am._invertedLists["sch"].size());
  ASSERT_EQ(0, am._invertedLists["#aap"].size());
  ASSERT_EQ(1, am._invertedLists["p##"].size());
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, ComputeApproximateMatches) {
  using std::vector;
  using std::string;
  ApproximateMatching am;
  am.BuildIndex("./simple.txt", 3);
  ASSERT_EQ(45, am._words.size());
  ASSERT_EQ(3, am._k);
  /*vector<const string*> res1 = am.ComputeApproximateMatches("aantekeningen");
  ASSERT_EQ(2, res1.size());
  ASSERT_EQ("aantekeningen", *res1[1]);
  ASSERT_EQ("aanteekeningen", *res1[0]);*/
  vector<const string*> res2 = am.ComputeApproximateMatches("aap");
  ASSERT_EQ(10, res2.size());
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, ComputeApproximateMatchesOld) {
  using std::vector;
  using std::string;
  ApproximateMatching am;
  am.BuildIndex("./simple.txt", 3);
  ASSERT_EQ(45, am._words.size());
  ASSERT_EQ(3, am._k);
  vector<const string*> res1 = am.ComputeApproximateMatchesOld("aantekeningen");
  ASSERT_EQ(2, res1.size());
  ASSERT_EQ("aantekeningen", *res1[1]);
  ASSERT_EQ("aanteekeningen", *res1[0]);
  vector<const string*> res2 = am.ComputeApproximateMatchesOld("aap");
  ASSERT_EQ(10, res2.size());
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, computeEditDistance) {
  ApproximateMatching am;
  ASSERT_EQ(3, am.computeEditDistance("board", "bread"));
  ASSERT_EQ(3, am.computeEditDistance("bread", "board"));
  ASSERT_EQ(1, am.computeEditDistance("islander", "slander"));
  ASSERT_EQ(1, am.computeEditDistance("slander", "islander"));
  ASSERT_EQ(3, am.computeEditDistance("mart", "karma"));
  ASSERT_EQ(3, am.computeEditDistance("karma", "mart"));
  ASSERT_EQ(3, am.computeEditDistance("abcca", "babda"));
  ASSERT_EQ(5, am.computeEditDistance("execution", "intention"));
  ASSERT_EQ(6, am.computeEditDistance("a", "#abcdef"));
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, merge) {
  using std::vector;
  ApproximateMatching am;
  vector<size_t> a = {1, 3, 7, 13, 15, 23, 33, 34, 45, 54, 78, 98, 101};
  vector<size_t> b = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 23, 33};
  vector<size_t> c = {1, 2, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 23, 33, 55};
  vector<size_t> d = {3, 4, 5, 6, 7, 8, 10, 11, 23, 34, 45, 55};
  vector<const vector<size_t>*> lists;
  lists.push_back(&a);
  lists.push_back(&b);
  lists.push_back(&c);
  lists.push_back(&d);
  vector<size_t> res = am.merge(lists);
  size_t resultSize = a.size() + b.size() + c.size() + d.size();
  ASSERT_EQ(resultSize, res.size());
  ASSERT_EQ(1, res[0]);
  ASSERT_EQ(1, res[1]);
  ASSERT_EQ(2, res[2]);
  ASSERT_EQ(2, res[3]);
  ASSERT_EQ(3, res[4]);
  ASSERT_EQ(3, res[5]);
  ASSERT_EQ(3, res[6]);
  ASSERT_EQ(4, res[7]);
  ASSERT_EQ(4, res[8]);
  ASSERT_EQ(98, res[res.size()-2]);
  ASSERT_EQ(101, res[res.size()-1]);
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, intersectInvertedLists) {
  using std::vector;
  ApproximateMatching am;
  vector<size_t> a = {1, 3, 7, 13, 15, 23, 33, 34, 45, 54, 78, 98, 101};
  vector<size_t> b = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 23, 33};
  vector<size_t> c = {1, 2, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, 16, 23, 33, 55};
  vector<size_t> d = {3, 4, 5, 6, 7, 8, 10, 11, 23, 34, 45, 55};
  vector<const vector<size_t>*> lists;
  lists.push_back(&a);
  lists.push_back(&b);
  lists.push_back(&c);
  lists.push_back(&d);
  vector<size_t> res = am.intersectInvertedLists(lists);
  ASSERT_EQ(2, res.size());
  ASSERT_EQ(7, res[0]);
  ASSERT_EQ(23, res[1]);
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, mergeInvertedLists) {
  using std::vector;
  ApproximateMatching am;
  vector<size_t> a = {1, 3, 7, 14, 21, 26};
  vector<size_t> b = {2, 3, 4, 5, 33};
  vector<size_t> c = {1, 2, 4, 6, 7, 8};
  vector<size_t> d = {3, 4, 5, 33};
  vector<const vector<size_t>*> lists;
  lists.push_back(&a);
  lists.push_back(&b);
  lists.push_back(&c);
  lists.push_back(&d);
  vector<size_t> res = am.mergeInvertedLists(lists);
  ASSERT_EQ(12, res.size());
  ASSERT_EQ(1, res[0]);
  ASSERT_EQ(2, res[1]);
  ASSERT_EQ(3, res[2]);
  ASSERT_EQ(4, res[3]);
  ASSERT_EQ(5, res[4]);
  ASSERT_EQ(6, res[5]);
  ASSERT_EQ(7, res[6]);
  ASSERT_EQ(8, res[7]);
  ASSERT_EQ(14, res[8]);
  ASSERT_EQ(21, res[9]);
  ASSERT_EQ(26, res[10]);
  ASSERT_EQ(33, res[11]);
}
