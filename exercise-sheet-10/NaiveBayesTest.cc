// Copyright 2012, University of Freiburg
// Anton Stepan, Student of Computer Science
// <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <map>
#include <bitset>
#include <fstream>
#include <string>
#include <vector>
#include "./NaiveBayes.h"

using std::string;
using std::vector;
using std::cout;
using std::endl;

// ____________________________________________________________________________
TEST(NaiveBayesTest, GetClassId) {
  NaiveBayes nb;
  nb._classes.push_back("Scientist");
  nb._classes.push_back("Biologist");
  ASSERT_EQ(0, nb.getClassId("Scientist"));
  ASSERT_EQ(1, nb.getClassId("Biologist"));
  ASSERT_EQ(2, nb.getClassId("Matchematician"));
  nb.ReadDocumentsAndLabels("wiki-test.csv");
}

// ____________________________________________________________________________
TEST(NaiveBayesTest, GetWordId) {
  NaiveBayes nb;
  nb._words.push_back("Scientist");
  nb._words.push_back("Biologist");
  ASSERT_EQ(0, nb.getClassId("Scientist"));
  ASSERT_EQ(1, nb.getClassId("Biologist"));
  ASSERT_EQ(2, nb.getClassId("Matchematician"));
  nb.ReadDocumentsAndLabels("wiki-test.csv");
}

// ____________________________________________________________________________
TEST(NaiveBayesTest, ReadDocumentsAndLabels) {
  {
    NaiveBayes nb;
    nb.ReadDocumentsAndLabels("wiki-test.csv");
    ASSERT_EQ(nb._classes[0], "Biologist");
    ASSERT_EQ(nb._classes[1], "Anthropologist");
    ASSERT_EQ(nb._classes[2], "Social_Scientist");
    ASSERT_EQ(nb._classes[3], "Mathematician");
    ASSERT_EQ(nb._classes[4], "Physicist");
  }
  {
    NaiveBayes nb;
    nb.ReadDocumentsAndLabels("wiki-test-simple.csv");
    ASSERT_EQ(nb._classes[0], "Scientist");
    ASSERT_EQ(nb._documentClasses[0].size(), 1);
    ASSERT_EQ(nb._documentClasses[0][0], 0);
    ASSERT_EQ(nb._classes[1], "Mathematician");
    ASSERT_EQ(nb._documentClasses[1].size(), 1);
    ASSERT_EQ(nb._documentClasses[1][0], 1);
    ASSERT_EQ(nb._classes[2], "Artist");
    ASSERT_EQ(nb._documentClasses[2].size(), 1);
    ASSERT_EQ(nb._documentClasses[2][0], 2);
    ASSERT_EQ(nb._words[0], "hello");
    ASSERT_EQ(nb._words[1], "world");
    ASSERT_EQ(nb._words[2], "here");
    ASSERT_EQ(nb._words[3], "come");
    ASSERT_EQ(nb._words[4], "the");
    ASSERT_EQ(nb._words[5], "yours");
  }
}

// ____________________________________________________________________________
TEST(NaiveBayesTest, train) {
  NaiveBayes nb;
  nb.ReadDocumentsAndLabels("wiki-test.csv");
  vector<int> ids = {0, 1, 2};
  nb.train(ids);
}
