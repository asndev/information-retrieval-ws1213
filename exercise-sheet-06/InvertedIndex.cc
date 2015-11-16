// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#include "./InvertedIndex.h"
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include "./Posting.h"

using std::ifstream;
using std::map;
using std::unordered_map;
using std::string;
using std::vector;

const size_t minWordLength = 1;

// _____________________________________________________________________________
vector<string> InvertedIndex::getAllWords() {
  vector<std::pair<int, string>> tempResult;
  // Sort results on documentFrequencies.
  for (auto it = _invertedLists.begin(); it != _invertedLists.end(); ++it) {
    tempResult.push_back(make_pair(_documentFrequencies[it->first], it->first));
  }
  std::sort(tempResult.begin(), tempResult.end(),
    [](const std::pair<int, string>& lhs,
       const std::pair<int, string>& rhs) {
            return lhs.first > rhs.first;
      });
  vector<string> result;
  result.reserve(tempResult.size());
  for (auto it = tempResult.begin(); it != tempResult.end(); ++it) {
    result.push_back(it->second);
  }
  return result;
}

// _____________________________________________________________________________
void InvertedIndex::buildFromCsvFile(string fileName, float bm25k,
                                     float bm25b) {
  ifstream file(fileName.c_str());
  string line;
  int docId = 0;
  map<string, bool> wordInDocument;
  string prvUrl = "";
  while (true) {
    getline(file, line);
    if (file.eof()) break;
    size_t pos = 0;
    pos = line.find('\t', pos);
    // line in wrong format?
    if (pos == string::npos) { perror("Wrong format"); }
    string url = line.substr(0, pos);
    string content = line.substr(pos+1);
    // to lowercase
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    if (prvUrl != url) {
      _urls.push_back(url);
      _records.push_back(content);
      docId++;
      _documentLengths.push_back(0);
      wordInDocument.clear();
      prvUrl = url;
    } else {
      _records.back() += " " + content;
    }
    // Parse the line and add each word to the respective index list.
    const size_t linesize = line.size();
    while (pos < linesize) {
      while (pos < linesize && !isalpha(line[pos])) pos++;
      size_t wordStart = pos;
      while (pos < linesize && isalpha(line[pos])) pos++;
      size_t wordEnd = pos;
      if (wordEnd >= wordStart + minWordLength) {
        string word = line.substr(wordStart, wordEnd - wordStart);
        Posting posting(docId, 1.0f);
        // Insert Word or increment score (need faster algorithm).
        if (_invertedLists.count(word) &&
           (_invertedLists[word].back().docId == static_cast<int>(docId))
        ) {
          _invertedLists[word].back().score++;
        } else {
          _invertedLists[word].push_back(posting);
        }
        _documentLengths[docId - 1] += 1;
        // If the word occures the first time in this document, add
        // 1 to the document frequencie for this word.
        if (!wordInDocument[word]) {
          wordInDocument[word] = true;
          !_documentFrequencies[word] ? _documentFrequencies[word] = 1
                                      : _documentFrequencies[word] += 1;
        }
      }
    }
  }
  //  Generate Scores
  float AVDL = 0;
  float totalNrOfDocuments = _urls.size();
  auto docLengthBegin = _documentLengths.begin();
  auto docLengthEnd = _documentLengths.end();
  for (vector<int>::const_iterator it = docLengthBegin;
       it != docLengthEnd; ++it) {
    AVDL += (*it);
  }
  AVDL = AVDL/static_cast<float>(totalNrOfDocuments);
  for (unordered_map<string, vector<Posting> >::iterator it =
       _invertedLists.begin(); it != _invertedLists.end(); ++it) {
    const string& word = it->first;
    float logWordScore = std::log2(totalNrOfDocuments
        / static_cast<float>(_documentFrequencies[word]));
    for (vector<Posting>::iterator pit = it->second.begin();
         pit != it->second.end(); ++pit) {
      float tf = pit->score;
      float score = tf * (bm25k+1);
      score /= (bm25k * (1 - bm25b + bm25b *
                static_cast<float>(_documentLengths[(*pit).docId - 1])
                / AVDL) + tf);
      assert(totalNrOfDocuments >= _documentFrequencies[word]);
      score *= logWordScore;
      (*pit).score = score;
    }
  }
}

// _____________________________________________________________________________
const vector<Posting>& InvertedIndex::getPostingForWord(const string& w) const {
  static vector<Posting> _empty;
  auto const it = _invertedLists.find(w);
  if (it == _invertedLists.cend()) {
    return _empty;
  }
  return it->second;
}
