// Copyright 2012, University of Freiburg
// Anton Stepan, Student of Computer Science
// <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>
#include "./InvertedIndex.h"
#include "./Posting.h"

using std::string;
using std::vector;
using std::map;

// ____________________________________________________________________________
TEST(InvertedIndexTest, invertedIndex) {
  InvertedIndex ii;
  ii.buildFromCsvFile("./simple_for_ii.csv", 1.75f, 0.75f);
  ASSERT_EQ(9, ii._documentLengths[0]);
  ASSERT_EQ(6, ii._documentLengths[1]);
  ASSERT_EQ(9, ii._documentLengths[2]);
  ASSERT_EQ(1, ii._documentFrequencies["this"]);
  ASSERT_EQ(0, ii._documentFrequencies["blablabla"]);
  ASSERT_EQ(1, ii._documentFrequencies["website"]);
  ASSERT_EQ(2, ii._documentFrequencies["is"]);

  Posting p1(2, 0.664216);
  Posting p2(3, 0.552029);
  vector<Posting> v = ii._invertedLists["lots"];
  ASSERT_EQ(2, v.size());
  ASSERT_EQ(p1.docId, v[0].docId);
  ASSERT_NEAR(p1.score, v[0].score, 0.0001);
  ASSERT_EQ(p2.docId, v[1].docId);
  ASSERT_NEAR(p2.score, v[1].score, 0.0001);
}

// ____________________________________________________________________________
TEST(InvertedIndexTest, getDocsForId) {
  InvertedIndex ii;
  ii.buildFromCsvFile("./simple_for_ii.csv", 1.75f, 0.75f);
  vector<Posting> vp = ii.getPostingForWord("lots");
  ASSERT_EQ(2, vp.size());
  vector<Posting> vp2 = ii.getPostingForWord("is");
  for (vector<Posting>::const_iterator it = vp2.begin();
       it != vp2.end(); ++it) {
  }
  ASSERT_EQ(2, vp2.size());
  vector<Posting> vp3 = ii.getPostingForWord("blabla");
  ASSERT_EQ(0, vp3.size());
}
