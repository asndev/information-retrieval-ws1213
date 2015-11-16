// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>
#include <gtest/gtest.h>
#include <algorithm>
#include <string>
#include <vector>
#include "./ApproximateMatching.h"

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, BuildIndex) {
  {
    ApproximateMatching am;
    am.BuildIndex("./simple.txt", 3);
    ASSERT_EQ(3, am._k);
    ASSERT_EQ(45, am._words.size());
    ASSERT_EQ(10, am._invertedLists["aap"].size());
    ASSERT_EQ(0, am._invertedLists["xxx"].size());
    ASSERT_EQ(44, am._invertedLists["#aa"].size());
    ASSERT_EQ(1, am._invertedLists["sch"].size());
    ASSERT_EQ(0, am._invertedLists["#aap"].size());
  }
  {
    using std::vector;
    using std::string;
    vector<string> words;
    words.push_back("hello");
    words.push_back("word");
    words.push_back("here");
    words.push_back("we");
    words.push_back("gogogo");
    ApproximateMatching am;
    am.BuildIndex(words, 3);
    ASSERT_EQ(3, am._k);
    ASSERT_EQ(5, am._words.size());
    ASSERT_EQ(2, am._invertedLists["##h"].size());
    ASSERT_EQ(0, am._invertedLists["abc"].size());
    ASSERT_EQ(1, am._invertedLists["wor"].size());
    ASSERT_EQ(2, am._invertedLists["##w"].size());
    ASSERT_EQ(2, am._invertedLists["#he"].size());
  }
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, computePrefixEditDistance) {
  ApproximateMatching am;
  ASSERT_EQ(1, am.computePrefixEditDistance("uniwe", "university"));
  ASSERT_EQ(1, am.computePrefixEditDistance("uniwe", "universe"));
  ASSERT_EQ(0, am.computePrefixEditDistance("ab", "ab"));
  ASSERT_EQ(2, am.computePrefixEditDistance("abcdefghjj", "abcdefghiiklmn"));
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, ComputeApproximateMatches) {
  using std::vector;
  using std::string;
  ApproximateMatching am;
  am.BuildIndex("./simple.txt", 3);
  ASSERT_EQ(45, am._words.size());
  ASSERT_EQ(3, am._k);
  vector<const string*> res1 = am.ComputeApproximateMatches("aan", 3);
  ASSERT_EQ(45, res1.size());
  vector<const string*> res2 = am.ComputeApproximateMatches("aanp", 1);
  ASSERT_EQ(26, res2.size());
  vector<const string*> res3 = am.ComputeApproximateMatches("aarde", 3);
  ASSERT_EQ(45, res3.size());
  vector<const string*> res5 = am.ComputeApproximateMatches("bar", 3);
  ASSERT_EQ(0, res5.size());

  ApproximateMatching am2;
  am2.BuildIndex("./simple2.txt", 3);
  ASSERT_EQ(8, am2._words.size());
  ASSERT_EQ(3, am2._k);
  vector<const string*> res4 = am2.ComputeApproximateMatches("abbdefgh", 2);
  ASSERT_EQ(4, res4.size());
}

// ____________________________________________________________________________
TEST(ApproximateMatchingTest, merge) {
  /*using std::vector;
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
  ASSERT_EQ(101, res[res.size()-1]);*/
}
