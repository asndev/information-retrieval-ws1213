// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#include "./InvertedIndex.h"
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

using std::ifstream;
using std::map;
using std::string;
using std::vector;

const size_t minWordLength = 2;

// _____________________________________________________________________________
void InvertedIndex::buildFromCsvFile(string fileName) {
  ifstream file(fileName.c_str());
  string line;
  size_t docId = 0;
  while (true) {
    getline(file, line);
    if (file.eof()) break;
    docId++;
    // Skip to second column (= after first tab).
    size_t pos = 0;
    pos = line.find('\t', pos);
    if (pos == std::string::npos) { perror("Wrong format"); }
    // copy the Url to the url vector and the record to the record vector
    _urls.push_back(line.substr(0, pos));
    _records.push_back(line.substr(pos+1));
    // Parse the line and add each word to the respective index list.
    while (pos < line.size()) {
      while (pos < line.size() && !isalpha(line[pos])) pos++;
      size_t wordStart = pos;
      while (pos < line.size() && isalpha(line[pos])) pos++;
      size_t wordEnd = pos;
      if (wordEnd >= wordStart + minWordLength) {
        string word = line.substr(wordStart, wordEnd - wordStart);
        _invertedLists[word].push_back(docId);
      }
    }
  }
}

// _____________________________________________________________________________
void InvertedIndex::outputInvertedListLengths() {
  for (map<string, vector<int> >::iterator it = _invertedLists.begin();
       it != _invertedLists.end(); ++it) {
    string word = it->first;
    size_t length = it->second.size();
    std::cout << word << "\t" << length << std::endl;
  }
}

// _____________________________________________________________________________
string InvertedIndex::getRecordAt(int id) {
  if (id > static_cast<int>(_records.size())) { perror("Index out of bounds"); }
  return _records.at(id);
}

// _____________________________________________________________________________
string InvertedIndex::getUrlAt(int id) {
  if (id > static_cast<int>(_urls.size())) { perror("Index out of bounds"); }
  return _urls.at(id);
}

// _____________________________________________________________________________
vector<int> InvertedIndex::getDocIdsForWord(string word) {
  vector<int> result;
  if (_invertedLists[word].size() > 0) {
    result = _invertedLists[word];
  }
  return result;
}

// _____________________________________________________________________________
void InvertedIndex::setInvertedLists(map<string, vector<int> > il) {
  _invertedLists = il;
}

// _____________________________________________________________________________
void InvertedIndex::setRecords(vector<string> records) {
  _records = records;
}

// _____________________________________________________________________________
void InvertedIndex::setUrls(vector<string> urls) {
  _urls = urls;
}
