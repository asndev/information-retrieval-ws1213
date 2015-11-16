// Copyright 2012, University of Freiburg
// Anton Stepan, Student of Computer Science
// <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>
#include "./InvertedIndex.h"

using std::string;
using std::vector;
using std::map;

InvertedIndex createIndex() {
  InvertedIndex ii;

  vector<string> testRecords;
  testRecords.push_back("who was it");
  testRecords.push_back("he was it");

  vector<string> testUrls;
  testUrls.push_back("http://test1.com");
  testUrls.push_back("http://test2.com");

  map<string, vector<int> > testMap;
  testMap["who"].push_back(1);
  testMap["was"].push_back(1);
  testMap["it"].push_back(1);
  testMap["he"].push_back(2);
  testMap["was"].push_back(2);
  testMap["it"].push_back(2);

  ii.setRecords(testRecords);
  ii.setUrls(testUrls);
  ii.setInvertedLists(testMap);

  return ii;
}

// ____________________________________________________________________________
TEST(InvertedIndexTest, getRecordAt) {
  InvertedIndex ii = createIndex();
  ASSERT_EQ("who was it", ii.getRecordAt(0));
  ASSERT_EQ("he was it", ii.getRecordAt(1));
}

// ____________________________________________________________________________
TEST(InvertedIndexTest, getUrlAt) {
  InvertedIndex ii = createIndex();
  ASSERT_EQ("http://test1.com", ii.getUrlAt(0));
  ASSERT_EQ("http://test2.com", ii.getUrlAt(1));
}

TEST(InvertedIndexTest, getDocIdsForWord) {
  InvertedIndex ii = createIndex();
  vector<int> v1 = ii.getDocIdsForWord("was");
  vector<int> v2 = ii.getDocIdsForWord("he");
  ASSERT_EQ(2, v1.size());
  ASSERT_EQ(1, v1[0]);
  ASSERT_EQ(2, v1[1]);
  ASSERT_EQ(1, v2.size());
  ASSERT_EQ(2, v2[0]);
}
