// Copyright 2012 Anton Stepan <stepana@informatik.uni-freiburg.de>
#ifndef EXERCISE_SHEET_07_SEARCHSERVER_H_
#define EXERCISE_SHEET_07_SEARCHSERVER_H_

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "./ApproximateMatching.h"
#include "./InvertedIndex.h"

// Class implementing the search backend.
class SearchServer {
 public:
  // Constructor. Needs the two index data structures (see private members
  // below) and the port on which to listen. In C++, make sure that you pass the
  // first two arguments by reference and not by value (in the latter case, they
  // would be copied).
  SearchServer(const ApproximateMatching& am,
               const InvertedIndex& ii,
               const size_t port);
  FRIEND_TEST(SearchServerTest, constructor);

  // Method providing the server loop. Note that this should support two types
  // of queries:
  // (1) for a given (partial) query word, get the top approximate
  // completions via the am object below;
  // (2) for a given query word, get the top matching documents via the inverted
  // index below.
  void ServerLoop();
 private:
  // Data structure for answering error-tolerant prefix queries, see
  // ApproximateMatching.H.
  ApproximateMatching _am;

  // Inverted index with BM25 scores for answering keyword queries.
  InvertedIndex _ii;

  size_t _port;
};

#endif  // EXERCISE_SHEET_07_SEARCHSERVER_H_
