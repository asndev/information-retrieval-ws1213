// Copyright 2013, Anton Stepan

#ifndef EXERCISE_SHEET_10_NAIVEBAYES_H_
#define EXERCISE_SHEET_10_NAIVEBAYES_H_

#include <gtest/gtest.h>
#include <set>
#include <string>
#include <vector>

// A class implementing Naive Bayes learning and prediction for text records
// (documents).
class NaiveBayes {
 public:
  // Read documents from CSV file. Note that, unlike in previous exercises,
  // there is now one line per whole document (not per sentence), and an
  // additional column containing the class label. Namely, column 1 = URL,
  // column 2 = class label, column 3 = text (all sentences of the document from
  // that URL concatenated).
  // NOTE: This method sets the member variables
  // documentVectors, documentClasses, words, and classes below.
  void ReadDocumentsAndLabels(std::string csvFileName);
  FRIEND_TEST(NaiveBayesTest, ReadDocumentsAndLabels);
  // Learn prior probabilities from the documents with the given ids (which
  // refer to documentVectors below).
  // NOTE: This method sets the member variables pc and pwc from below.
  void train(std::vector<int> DocumentIds);
  FRIEND_TEST(NaiveBayesTest, train);
  // Predict classes for the documents with the given ids. Returns for each of
  // these documents the class label with the highest probability (the argument
  // array and the returned array are of the same size).
  std::vector<int> predict(std::vector<int> DocumentIds);
 private:
  int getClassId(const std::string& s);
  FRIEND_TEST(NaiveBayesTest, GetClassId);
  int getWordId(const std::string& s);
  FRIEND_TEST(NaiveBayesTest, GetWordId);
  // The document vectors, where each document vector simply contains the ids of
  // the words it contains.
  std::vector<std::vector<int> > _documentVectors;
  // The ids of the classes each document is assigned to in the CSV file.
  std::vector<std::vector<int> > _documentClasses;
  // For each word id, the actual word.
  std::vector<std::string> _words;
  // For each class id, the actual name of the class.
  std::vector<std::string> _classes;
  // Prior probabilities, learned by the method train(...) above. The first are
  // the Prob(C = c) for each class, the second are the Prob(W = w | C = c) for
  // each class and each word.
  std::vector<double> _pc;
  std::vector<std::vector<double> > _pwc;
};

#endif  // EXERCISE_SHEET_10_NAIVEBAYES_H_
