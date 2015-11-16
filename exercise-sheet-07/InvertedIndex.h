// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#ifndef EXERCISE_SHEET_07_INVERTEDINDEX_H_
#define EXERCISE_SHEET_07_INVERTEDINDEX_H_

#include <gtest/gtest.h>
#include <unordered_map>
#include <map>
#include <string>
#include <vector>
#include "./Posting.h"

// Class implementing an inverted index (INV).
class InvertedIndex {
 public:
  // Create index from a text collection in CSV format (one record per line,
  // two columns, column 1 = URL, column 2 = text).
  void buildFromCsvFile(string fileName, float bm25k, float bm25b);

  std::vector<char> getValidUtf(std::string actPoint);

  // returns the Postings for a given word in the inverted index.
  const std::vector<Posting>& getPostingForWord(const std::string& w) const;

  const std::string& getRecord(size_t id) const { return _records.at(id); }
  const std::string& getUrl(size_t id) const { return _urls.at(id); }
  std::vector<std::string> getAllWords();
  std::string utf8Repair(const std::string& word);
 private:
  // The inverted lists = a sorted list of record ids for each word in the
  // collection.
  std::unordered_map<std::string, std::vector<Posting>> _invertedLists;
  FRIEND_TEST(InvertedIndexTest, invertedIndex);

  std::map<std::string, int> _documentFrequencies;
  std::vector<int> _documentLengths;

  // Contents and urls from the documents
  // next exercise will contain a record struct
  std::vector<std::string> _urls;
  std::vector<std::string> _records;
  std::map<int, std::vector<int> > _docIds;
};

#endif  // EXERCISE_SHEET_07_INVERTEDINDEX_H_
