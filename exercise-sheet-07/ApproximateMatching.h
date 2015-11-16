// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>
#ifndef EXERCISE_SHEET_07_APPROXIMATEMATCHING_H_
#define EXERCISE_SHEET_07_APPROXIMATEMATCHING_H_

#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <vector>

class ApproximateMatching {
 public:
  // Build a k-gram index for a given vocabulary (one word per line).
  // Pad k-1 times # to the left of each word.
  void BuildIndex(const std::string& filename, const size_t k);
  void BuildIndex(const std::vector<std::string>& words, const size_t k);
  // Find all words that are within the given edit distance.
  std::vector<const std::string*> ComputeApproximateMatches(
    const std::string& word, const size_t maxEditDistance);
 private:
  std::vector<std::string> buildKGrams(const std::string& word);
  // Compute the edit distance between the two given words using the O(|word1| *
  // |word2|) dynamic programming algorithm explained in the lecture.
  size_t computePrefixEditDistance(const std::string& word1,
                             const std::string& word2) const;
  // Computes the union of the given inverted k-gram lists with duplicates
  std::vector<size_t> merge(
    const std::vector<const std::vector<size_t>*>& lists) const;

  // The inverted lists for each k-gram. Words are represented by their word
  // ids.
  std::unordered_map<std::string, std::vector<size_t>> _invertedLists;
  // Mapping of ids to words.
  std::vector<std::string> _words;
  // k of the k-Gram
  size_t _k;

  // Unittests can access private variables.
  FRIEND_TEST(ApproximateMatchingTest, ComputeApproximateMatches);
  FRIEND_TEST(ApproximateMatchingTest, computePrefixEditDistance);
  FRIEND_TEST(ApproximateMatchingTest, merge);
  FRIEND_TEST(ApproximateMatchingTest, BuildIndex);
};

#endif  // EXERCISE_SHEET_07_APPROXIMATEMATCHING_H_
