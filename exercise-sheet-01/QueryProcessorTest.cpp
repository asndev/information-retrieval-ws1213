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
TEST(QueryProcessorTest, answerSingleQuery) {
  InvertedIndex ii = createIndex();
  QueryProcessor qp(ii);
  vector<int> vi1 = qp.answerSingleQuery("was");
  ASSERT_EQ(2, vi1.size());
  ASSERT_EQ(1, vi1.at(0));
  ASSERT_EQ(2, vi1.at(1));
}

// ____________________________________________________________________________
TEST(QueryProcessorTest, answerQuery) {
  InvertedIndex ii = createIndex();
  QueryProcessor qp(ii);
  vector<string> result = qp.answerQuery("was it", 3);
  ASSERT_EQ("http://test1.com\twho was it", result.at(0));
  ASSERT_EQ("http://test2.com\the was it", result.at(1));
}

// ____________________________________________________________________________
TEST(QueryProcessorTest, intersect) {
  vector<int> v1;
  v1.push_back(1);
  v1.push_back(2);
  v1.push_back(3);
  v1.push_back(5);
  v1.push_back(6);
  v1.push_back(13);
  v1.push_back(17);
  v1.push_back(20);

  vector<int> v2;
  v2.push_back(3);
  v2.push_back(6);
  v2.push_back(7);
  v2.push_back(9);
  v2.push_back(10);
  v2.push_back(13);
  v2.push_back(20);

  InvertedIndex ii = createIndex();
  QueryProcessor qp(ii);

  vector<int> result = qp.intersect(v1, v2);
  ASSERT_EQ(4, result.size());
  ASSERT_EQ(3, result.at(0));
  ASSERT_EQ(6, result.at(1));
  ASSERT_EQ(13, result.at(2));
  ASSERT_EQ(20, result.at(3));
}
