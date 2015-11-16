// Copyright 2012, University of Freiburg
// Anton Stepan, Student of Computer Science
// <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <map>
#include <bitset>
#include <fstream>
#include <string>
#include <vector>
#include "./InvertedIndex.h"
#include "./Posting.h"

using std::string;
using std::vector;
using std::map;
using std::cout;
using std::endl;

// ____________________________________________________________________________
TEST(InvertedIndexTest, WriteTermDocumentMatrix) {
  InvertedIndex ii;
  ii.buildFromCsvFile("./simple_for_ii.csv", 1.75f, 0.75f);
  ASSERT_EQ(9, ii._documentLengths[0]);
  ASSERT_EQ(6, ii._documentLengths[1]);
  ASSERT_EQ(9, ii._documentLengths[2]);
  ASSERT_EQ(1, ii._documentFrequencies["this"]);
  ASSERT_EQ(0, ii._documentFrequencies["blablabla"]);
  ASSERT_EQ(1, ii._documentFrequencies["website"]);
  ASSERT_EQ(2, ii._documentFrequencies["is"]);

  ii.WriteTermDocumentMatrix("testoutput", 10);
}

// ____________________________________________________________________________
TEST(InvertedIndexTest, utf8repair) {
  InvertedIndex ii;
  string carrental = "carrental";
  string ueei = "Überraschungsei";
  ASSERT_EQ(carrental, ii.utf8Repair(carrental));
  ASSERT_EQ(ueei, ii.utf8Repair(ueei));
}

// ____________________________________________________________________________
TEST(InvertedIndexTest, getValidUtf) {
  InvertedIndex ii;
  string test1 = "01111000";
  vector<char> test1v;
  test1v.push_back(static_cast<char>(std::bitset<8>(test1).to_ulong()));
  ASSERT_EQ(test1v, ii.getValidUtf(test1));
  string test2_1 = "11000011";
  string test2_2 = "10011100";
  vector<char> test2v;
  test2v.push_back(static_cast<char>(std::bitset<8>(test2_1).to_ulong()));
  test2v.push_back(static_cast<char>(std::bitset<8>(test2_2).to_ulong()));
  ASSERT_EQ(test2v, ii.getValidUtf("00011011100"));
  string test3_1 = "11101011";
  string test3_2 = "10011100";
  string test3_3 = "10101110";
  vector<char> test3v;
  test3v.push_back(static_cast<char>(std::bitset<8>(test3_1).to_ulong()));
  test3v.push_back(static_cast<char>(std::bitset<8>(test3_2).to_ulong()));
  test3v.push_back(static_cast<char>(std::bitset<8>(test3_3).to_ulong()));
  ASSERT_EQ(test3v, ii.getValidUtf("1011011100101110"));
  string test4_1 = "11110111";
  string test4_2 = "10011100";
  string test4_3 = "10101110";
  string test4_4 = "10101110";
  vector<char> test4v;
  test4v.push_back(static_cast<char>(std::bitset<8>(test4_1).to_ulong()));
  test4v.push_back(static_cast<char>(std::bitset<8>(test4_2).to_ulong()));
  test4v.push_back(static_cast<char>(std::bitset<8>(test4_3).to_ulong()));
  test4v.push_back(static_cast<char>(std::bitset<8>(test4_4).to_ulong()));
  ASSERT_EQ(test4v, ii.getValidUtf("111011100101110101110"));
}

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
