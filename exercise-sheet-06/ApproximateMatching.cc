// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>
#include "./ApproximateMatching.h"
#include <unordered_map>
#include <chrono>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

using std::string;
using std::vector;

// _____________________________________________________________________________
void ApproximateMatching::BuildIndex(const vector<string>& words,
                                     const size_t k) {
  string line;
  _k = k;
  string pad(k-1, '#');
  size_t id = 0;
  const size_t listSize = words.size();
  while (id < listSize) {
    line = words[id];
    _words.push_back(line);
    line = pad + line;
    size_t start = 0;
    const size_t end = line.size() - k;
    while (start <= end) {
      _invertedLists[line.substr(start, k)].push_back(id);
      ++start;
    }
    ++id;
  }
}

// _____________________________________________________________________________
void ApproximateMatching::BuildIndex(const string& filename,
                                     const size_t k) {
  std::ifstream stream(filename.c_str());
  string line;
  _k = k;
  string pad(k-1, '#');
  size_t id = 0;
  while (true) {
    getline(stream, line);
    if (stream.eof()) break;
    // Line is a single word.
    _words.push_back(line);
    line = pad + line;
    size_t start = 0;
    const size_t end = line.size() - k;
    while (start <= end) {
      _invertedLists[line.substr(start, k)].push_back(id);
      ++start;
    }
    ++id;
  }
  stream.close();
}

// _____________________________________________________________________________
size_t ApproximateMatching::computePrefixEditDistance(
  const std::string& word1, const std::string& word2) const {
  const size_t xLength = word1.size()+1;
  const size_t yLength = word2.size()+1;
  size_t matrix[xLength][yLength];
  // Initialize first col/row.
  for (size_t i = 0; i < xLength; ++i) { matrix[i][0] = i; }
  for (size_t i = 0; i < yLength; ++i) { matrix[0][i] = i; }

  // Compute edit distance.
  for (size_t i = 1; i < xLength; ++i) {
    for (size_t j = 1; j < yLength; ++j) {
      size_t entry = std::min(matrix[i][j-1] + 1, matrix[i-1][j] + 1);
      size_t diag = (word1[i-1] == word2[j-1]) ?
                    matrix[i-1][j-1] : matrix[i-1][j-1] + 1;
      matrix[i][j] = std::min(entry, diag);
    }
  }
  const size_t delimiter = xLength-1;
  size_t res = matrix[delimiter][0];
  for (size_t i = 1; i < yLength; ++i) {
    res = std::min(res, matrix[delimiter][i]);
  }
  return res;
}

// _____________________________________________________________________________
vector<const string*> ApproximateMatching::ComputeApproximateMatches(
  const string& word, const size_t maxEditDistance) {
  const size_t wordSize = word.size();
  vector<string> kgrams = buildKGrams(word);
  vector<const vector<size_t>*> candidates;
  for (auto it = kgrams.begin(); it != kgrams.end(); ++it) {
    if (_invertedLists[(*it)].size()) {
      candidates.push_back(&_invertedLists[(*it)]);
    }
  }
  const vector<size_t> merged = merge(candidates);
  const size_t mergedSize = merged.size();
  vector<const string*> result;
  result.reserve(mergedSize);
  size_t j = 0;
  while (j < mergedSize) {
    size_t count = 1;
    while (merged[j] == merged[j+1]) {
      ++count;
      ++j;
    }
    string& possibleWord = _words[merged[j]];
    int threshold = wordSize - maxEditDistance*_k;
    if (static_cast<int>(count) >= threshold) {
      if (computePrefixEditDistance(word, possibleWord) <= maxEditDistance) {
        result.push_back(&possibleWord);
      }
    }
    ++j;
  }
  return result;
}

// _____________________________________________________________________________
vector<size_t> ApproximateMatching::merge(
    const vector<const vector<size_t>*>& lists) const {
  using std::pair;
  const size_t numLists = lists.size();
  vector<size_t> pos(numLists, 0);
  // PQ having an element for each list and the smallest at top.
  std::priority_queue<pair<size_t, size_t>, vector<pair<size_t, size_t> >,
                      std::greater<pair<size_t, size_t>> > prio;
  // Pushing the first element of each list into the priority queue.
  size_t allListsSpace = 0;
  for (size_t i = 0; i < numLists; ++i) {
    prio.push(std::make_pair((*lists[i])[0], i));
    allListsSpace += (*lists[i]).size();
  }
  vector<size_t> result;
  result.reserve(allListsSpace);
  while (prio.size()) {
    const size_t actEle = prio.top().first;
    const size_t actList = prio.top().second;
    prio.pop();
    result.push_back(actEle);
    if (pos[actList] + 1u < lists[actList]->size()) {
      prio.push(std::make_pair(
        (*lists[actList])[++pos[actList]], actList));
    }
  }
  return result;
}

// _____________________________________________________________________________
vector<string> ApproximateMatching::buildKGrams(const string& word) {
  string pad(_k-1, '#');
  size_t start = 0;
  vector<string> kgrams;
  const string query = pad + word;
  const size_t end = query.size();
  while (start+_k <= end) {
    kgrams.push_back(query.substr(start, _k));
    ++start;
  }
  return kgrams;
}
