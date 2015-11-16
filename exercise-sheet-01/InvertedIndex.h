// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#ifndef EXERCISE_SHEET_01_INVERTEDINDEX_H_
#define EXERCISE_SHEET_01_INVERTEDINDEX_H_

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

// Class implementing an inverted index (INV).
class InvertedIndex {
 public:
  // Create index from a text collection in CSV format (one record per line,
  // two columns, column 1 = URL, column 2 = text).
  void buildFromCsvFile(string fileName);

  // sets an invertedindex to this class. Used by test cases.
  void setInvertedLists(map<string, vector<int> > il);

  // Set records and urls. Used by test cases.
  void setRecords(vector<string> records);
  void setUrls(vector<string> urls);

  // Write inverted lists lengths to standard output.
  void outputInvertedListLengths();

  // Getter for records and urls. Used by test cases.
  string getRecordAt(int id);
  string getUrlAt(int id);

  // returns the id's for a given word in the inverted index.
  vector<int> getDocIdsForWord(string word);

 private:
  // The inverted lists = a sorted list of record ids for each word in the
  // collection.
  map<string, vector<int> > _invertedLists;

  // the records from the given text collection.
  vector<string> _records;
  // the urls from the given text colelction.
  vector<string> _urls;
};

#endif  // EXERCISE_SHEET_01_INVERTEDINDEX_H_
