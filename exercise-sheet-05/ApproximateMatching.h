// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>
#ifndef EXERCISE_SHEET_05_APPROXIMATEMATCHING_H_
#define EXERCISE_SHEET_05_APPROXIMATEMATCHING_H_

#include <gtest/gtest.h>
#include <unordered_map>
#include <string>
#include <vector>

class ApproximateMatching {
 public:
  ApproximateMatching();
  // Build a k-gram index for a given vocabulary (one word per line).
  // Pad k-1 times # to the left of each word.
  void BuildIndex(const std::string& filename, const size_t k);
  // Find all words that are within the given edit distance of the given word.
  // Use the k-gram index and the mergeInvertedLists method to generate a list
  // of candidate matches. Then for each candidate use the computeEditDistance
  // method to determine whether it is an actual match. Returns the matching
  // words, not word ids (use the words Array below).
  std::vector<const std::string*> ComputeApproximateMatches(
    const std::string& word);
  FRIEND_TEST(ApproximateMatchingTest, ComputeApproximateMatches);
  std::vector<const std::string*> ComputeApproximateMatchesOld(
  const std::string& word);
  FRIEND_TEST(ApproximateMatchingTest, ComputeApproximateMatchesOld);
  // for timing of the EDs
  mutable double AvgEdTimeInNano;
  mutable size_t EdComputations;
 private:
  // Compute the edit distance between the two given words using the O(|word1| *
  // |word2|) dynamic programming algorithm explained in the lecture.
  size_t computeEditDistance(const std::string& word1,
                             const std::string& word2) const;
  FRIEND_TEST(ApproximateMatchingTest, computeEditDistance);
  // Computes the union of the given inverted k-gram lists with duplicates
  std::vector<size_t> merge(
    const std::vector<const std::vector<size_t>*>& lists) const;
  FRIEND_TEST(ApproximateMatchingTest, merge);
  // Computes the union of the given inverted k-gram lists and deletes
  // duplicates
  std::vector<size_t> mergeInvertedLists(
    const std::vector<const std::vector<size_t>*>& lists) const;
  FRIEND_TEST(ApproximateMatchingTest, mergeInvertedLists);
  // Computes the intersection of the given inverted k-gram lists.
  std::vector<size_t> intersectInvertedLists(
    const std::vector<const std::vector<size_t>*>& invertedLists) const;
  FRIEND_TEST(ApproximateMatchingTest, intersectInvertedLists);
  // The inverted lists for each k-gram. Words dare represented by their word
  // ids.
  std::unordered_map<std::string, std::vector<size_t>> _invertedLists;
  std::vector<std::string> buildKGrams(const std::string& word);
  // The mapping of word ids to words. If we choose word ids consecutively, we
  // can use a simple array for this.
  std::vector<std::string> _words;
  FRIEND_TEST(ApproximateMatchingTest, BuildIndex);
  // k of the k-Gram
  size_t _k;
};

#endif  // EXERCISE_SHEET_05_APPROXIMATEMATCHING_H_
