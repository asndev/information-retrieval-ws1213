// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>

#include <cassert>
#include <vector>
#include "./Intersect.h"

using std::vector;

// _____________________________________________________________________________
vector<int> Intersect::IntersectV1(const vector<int>& list1,
                                   const vector<int>& list2) {
  const size_t list1size = list1.size(), list2size = list2.size();
  const size_t smallerList = std::min(list1size, list2size);
  vector<int> result;
  result.reserve(smallerList * 1.5f);
  size_t i = 0, j = 0;
  while (i < list1size && j < list2size) {
    while (list1[i] < list2[j] && i < list1size) { ++i; }
    if (i == list1size) { break; }
    while (list1[i] > list2[j] && j < list2size) { ++j; }
    if (j == list2size) { break; }
    if (list1[i] == list2[j]) {
      result.push_back(list1[i]);
      ++i;
      ++j;
    }
  }
  assert(result.size() <= smallerList);
  return result;
}

// _____________________________________________________________________________
vector<int> Intersect::IntersectV2(const vector<int>& list1,
                                   const vector<int>& list2) {
  const size_t list1size = list1.size(), list2size = list2.size();
  const size_t smallerList = std::min(list1size, list2size);
  vector<int> result;
  result.reserve(smallerList * 1.5f);
  int lowerBound = 0;
  for (const int &e: list1) {
    int upperBound = ExponentialSearch(list2, lowerBound, e, list2size);
    int res = BinarySearch(list2, lowerBound, upperBound, e);
    if (res != -1) {
      result.push_back(list2[res]);
      lowerBound = res;
    }
  }
  assert(result.size() <= smallerList);
  return result;
}

int Intersect::BinarySearch(const vector<int>& list, int start,
                            int end, const int& key) {
  int mid;
  if (key < list[start] || key > list[end]) { return -1; }
  while (list[start] <= list[end]) {
    mid = (start + end) / 2;
    int midEle = list[mid];
    if (key > midEle) {
      start = mid + 1;
    } else if (key < midEle) {
      end = mid - 1;
    } else {
      assert(0 <= (size_t)mid && (size_t)mid <= list.size());
      return mid;
    }
  }
  return -1;
}

int Intersect::ExponentialSearch(const vector<int>& list, int pos,
                                 const int& key, const int& listSize) {
  int j = 1;
  assert(0 <= pos && pos < listSize);
  while (list[pos+j] < key && pos+j < listSize) {
    j *= 2;
  }
  int res = pos+j;
  if (res >= listSize) return listSize - 1;
  assert(pos < listSize);
  return res;
}
