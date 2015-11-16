// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>

#ifndef EXERCISE_SHEET_10_INTERSECT_H_
#define EXERCISE_SHEET_10_INTERSECT_H_

#include <gtest/gtest.h>
#include <vector>

class Intersect {
 public:
  static std::vector<int> IntersectV1(const std::vector<int>& list1,
                                      const std::vector<int>& list2);
  static std::vector<int> IntersectV2(const std::vector<int>& list1,
                                      const std::vector<int>& list2);
  static int BinarySearch(const std::vector<int>& list, int start,
                          int end, const int& key);
  static int ExponentialSearch(const std::vector<int>& list, int pos,
                               const int& key, const int& listSize);
};

#endif  // EXERCISE_SHEET_10_INTERSECT_H_
