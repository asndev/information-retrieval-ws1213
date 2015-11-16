// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>

#include <gtest/gtest.h>
#include <vector>
#include "./Intersect.h"

using std::vector;

// ____________________________________________________________________________
TEST(IntersectTEst, BinarySearch) {
  vector<int> list1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  ASSERT_EQ(4, Intersect::BinarySearch(list1, 0, list1.size()-1, 5));
  ASSERT_EQ(-1, Intersect::BinarySearch(list1, 0, list1.size()-1, 11));
  ASSERT_EQ(9, Intersect::BinarySearch(list1, 0, list1.size()-1, 10));
  ASSERT_EQ(0, Intersect::BinarySearch(list1, 0, list1.size()-1, 1));
}

// ____________________________________________________________________________
TEST(IntersectTEst, ExponentialSearch) {
  vector<int> list1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  ASSERT_EQ(8, Intersect::ExponentialSearch(list1, 0, 6, 10));
  ASSERT_EQ(9, Intersect::ExponentialSearch(list1, 0, 10, 10));
  ASSERT_EQ(1, Intersect::ExponentialSearch(list1, 0, 2, 10));
  ASSERT_EQ(2, Intersect::ExponentialSearch(list1, 0, 3, 10));
  ASSERT_EQ(1, Intersect::ExponentialSearch(list1, 0, 1, 10));
  ASSERT_EQ(9, Intersect::ExponentialSearch(list1, 0, 11, 10));
  ASSERT_EQ(5, Intersect::ExponentialSearch(list1, 4, 6, 10));
  ASSERT_EQ(9, Intersect::ExponentialSearch(list1, 3, 11, 10));
  ASSERT_EQ(9, Intersect::ExponentialSearch(list1, 5, 11, 10));
  ASSERT_EQ(9, Intersect::ExponentialSearch(list1, 7, 11, 10));
}

// ____________________________________________________________________________
TEST(IntersectTest, IntersectV1) {
  vector<int> list1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  vector<int> list2 = {0, 3, 7, 15};
  vector<int> result = Intersect::IntersectV1(list1, list2);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(vector<int>({3, 7}), result);

  list1 = {1, 2, 3};
  list2 = {4, 5, 6, 7};
  result = Intersect::IntersectV1(list1, list2);
  ASSERT_EQ(0, result.size());
  ASSERT_EQ(vector<int>({}), result);

  list1 = {};
  list2 = {4, 5, 6, 7};
  result = Intersect::IntersectV1(list1, list2);
  ASSERT_EQ(0, result.size());
  ASSERT_EQ(vector<int>({}), result);

  list1 = {1};
  list2 = {2};
  result = Intersect::IntersectV1(list1, list2);
  ASSERT_EQ(0, result.size());
  ASSERT_EQ(vector<int>({}), result);

  list1 = {1, 2, 3, 4, 5, 6, 50};
  list2 = {1, 50};
  result = Intersect::IntersectV1(list1, list2);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(vector<int>({1, 50}), result);

  list1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  list2 = {1, 3, 5, 7, 9, 11};
  result = Intersect::IntersectV1(list1, list2);
  ASSERT_EQ(5, result.size());
  ASSERT_EQ(vector<int>({1, 3, 5, 7, 9}), result);
}

// ____________________________________________________________________________
TEST(IntersectTest, IntersectV2) {
  vector<int> list1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  vector<int> list2 = {0, 3, 7, 15};
  vector<int> result = Intersect::IntersectV2(list1, list2);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(vector<int>({3, 7}), result);

  list1 = {1, 2, 3};
  list2 = {4, 5, 6, 7};
  result = Intersect::IntersectV2(list1, list2);
  ASSERT_EQ(0, result.size());
  ASSERT_EQ(vector<int>({}), result);

  list1 = {};
  list2 = {4, 5, 6, 7};
  result = Intersect::IntersectV2(list1, list2);
  ASSERT_EQ(0, result.size());
  ASSERT_EQ(vector<int>({}), result);

  list1 = {1};
  list2 = {2};
  result = Intersect::IntersectV2(list1, list2);
  ASSERT_EQ(0, result.size());
  ASSERT_EQ(vector<int>({}), result);

  list2 = {1, 2, 3, 4, 5, 6, 50};
  list1 = {1, 50};
  result = Intersect::IntersectV2(list1, list2);
  ASSERT_EQ(2, result.size());
  ASSERT_EQ(vector<int>({1, 50}), result);

  list2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  list1 = {1, 3, 5, 7, 9, 11};
  result = Intersect::IntersectV2(list1, list2);
  ASSERT_EQ(5, result.size());
  ASSERT_EQ(vector<int>({1, 3, 5, 7, 9}), result);
}
