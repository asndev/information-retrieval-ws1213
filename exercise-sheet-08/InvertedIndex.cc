// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#include "./InvertedIndex.h"
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
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

int binaryToDec(string number) {
  int result = 0, pow = 1, len = number.length() - 1;
  for ( int i = len; i >= 0; --i, pow <<= 1 )
    result += (number[i] - '0') * pow;
  return result;
}

void InvertedIndex::WriteTermDocumentMatrix(const std::string& filename,
                                            const size_t m) {
  using std::ofstream;
  using std::cout;
  using std::endl;
  ofstream outputmatrix, outputterms;
  string matrixFilename = filename + ".matrix";
  string termsFilename = filename + ".terms";
  std::remove(matrixFilename.c_str());
  std::remove(termsFilename.c_str());
  outputmatrix.open(matrixFilename);
  outputterms.open(termsFilename);
  if (!outputmatrix || !outputterms) {
    std::cerr << "Error: couldn't open files." << std::endl;
    exit(1);
  }
  // Sort the terms regarding their document frequencies.
  vector<std::pair<int, string>> docFreq;
  for (auto it = _documentFrequencies.begin(); it != _documentFrequencies.end();
       ++it) {
    docFreq.push_back(std::make_pair(it->second, it->first));
  }
  std::sort(docFreq.begin(), docFreq.end(),
    [](const std::pair<int, string>& lhs, const std::pair<int, string>& rhs) {
                  return lhs.first > rhs.first;
    });
  size_t counter = 1;
  assert(m <= docFreq.size() && "m is leq than docfreq");
  while (counter < m) {
    const string& word = docFreq[counter].second;
    outputterms << word << std::endl;
    auto postings = _invertedLists[word];
    for (Posting p : postings) {
      outputmatrix << counter << " " << p.docId << " " << p.score << std::endl;
    }
    ++counter;
  }
  outputterms.close();
  outputmatrix.close();
}

vector<char> InvertedIndex::getValidUtf(string actPoint) {
  using std::cout;
  using std::endl;
  vector<char> result;
  int binAsDec = binaryToDec(actPoint);
  if (binAsDec < 128) {
    result.push_back(static_cast<char>(std::bitset<8>(actPoint).to_ulong()));
  } else if (128 <= binAsDec && binAsDec < 2048) {
    result.push_back(
      static_cast<char>(
        std::bitset<8>("110" + actPoint.substr(0, 5)).to_ulong()));
    result.push_back(
      static_cast<char>(
        std::bitset<8>("10" + actPoint.substr(5)).to_ulong()));
  } else if (2048 <= binAsDec && binAsDec < 65535) {
    result.push_back(
      static_cast<char>(
        std::bitset<8>("1110" + actPoint.substr(0, 4)).to_ulong()));
    result.push_back(
      static_cast<char>(
        std::bitset<8>("10" + actPoint.substr(4, 6)).to_ulong()));
    result.push_back(
      static_cast<char>(
        std::bitset<8>("10" + actPoint.substr(10)).to_ulong()));
  } else if (65535 <= binAsDec &&
             binAsDec < static_cast<int>(std::pow(2, 21)) - 1) {
    result.push_back(
      static_cast<char>(
        std::bitset<8>("11110" + actPoint.substr(0, 3)).to_ulong()));
    result.push_back(
      static_cast<char>(
        std::bitset<8>("10" + actPoint.substr(3, 6)).to_ulong()));
    result.push_back(
      static_cast<char>(
        std::bitset<8>("10" + actPoint.substr(9, 6)).to_ulong()));
    result.push_back(
      static_cast<char>(
        std::bitset<8>("10" + actPoint.substr(15)).to_ulong()));
  } else {
    result.push_back(
      static_cast<char>(
        std::bitset<8>("00100000").to_ulong()));
  }
  assert(result.size() > 0);
  return result;
}

string InvertedIndex::utf8Repair(const string& word) {
  using std::cout;
  using std::endl;
  string result;
  size_t i = 0, size = word.size();
  while (i < size) {
    auto firstWord = std::bitset<8>(word[i]);
    assert(firstWord.size() == 8);
    size_t count = 0;
    while (firstWord[7 - count] == 1) {
      ++count;
    }
    if (count == 0) {
      result += word[i];
      ++i;
    } else if (count == 1) {
      result += " ";
      ++i;
    } else if (count == 2 && i+1 < word.size() &&
               std::bitset<8>(word[i+1]).test(7) &&
               !(std::bitset<8>(word[i+1]).test(6))) {
      auto secondWord = std::bitset<8>(word[i+1]);
      string actPoint = firstWord.to_string().substr(3);
      actPoint += secondWord.to_string().substr(2);
      vector<char> tmp = getValidUtf(actPoint);
      for (auto i : tmp) {
        result += i;
      }
      i += 2;
    } else if (count == 3 && i+2 < word.size() &&
               std::bitset<8>(word[i+1]).test(7) &&
               !(std::bitset<8>(word[i+1]).test(6)) &&
               std::bitset<8>(word[i+2]).test(7) &&
               !(std::bitset<8>(word[i+2]).test(6))) {
      auto secondWord = std::bitset<8>(word[i+1]);
      auto thirdWord = std::bitset<8>(word[i+2]);
      string actPoint = firstWord.to_string().substr(3);
      actPoint += secondWord.to_string().substr(2);
      actPoint += thirdWord.to_string().substr(2);
      vector<char> tmp = getValidUtf(actPoint);
      for (auto i : tmp) {
        result += i;
      }
      i += 3;
    } else if (count == 4 && i+3 < word.size() &&
               std::bitset<8>(word[i+1]).test(7) &&
               !(std::bitset<8>(word[i+1]).test(6)) &&
               std::bitset<8>(word[i+2]).test(7) &&
               !(std::bitset<8>(word[i+2]).test(6)) &&
               std::bitset<8>(word[i+3]).test(7) &&
               !(std::bitset<8>(word[i+3]).test(6))) {
      auto secondWord = std::bitset<8>(word[i+1]);
      auto thirdWord = std::bitset<8>(word[i+2]);
      auto forthWord = std::bitset<8>(word[i+3]);
      string actPoint = firstWord.to_string().substr(3);
      actPoint += secondWord.to_string().substr(2);
      actPoint += thirdWord.to_string().substr(2);
      actPoint += forthWord.to_string().substr(2);
      vector<char> tmp = getValidUtf(actPoint);
      for (auto i : tmp) {
        result += i;
      }
      i += 4;
    } else {
      result = "";
      ++i;
    }
  }
  return result;
}


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
  using std::cout;
  using std::endl;

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
    string url =  line.substr(0, pos);
    string content = line.substr(pos+1);
    // for utf8
    /*string oldUrl =  line.substr(0, pos);
    string oldContent = line.substr(pos+1);
    string url, content, buf;
    std::stringstream ss(oldUrl);
    while (ss >> buf)
      url += utf8Repair(buf) + " ";
    std::stringstream ss2(oldContent);
    while (ss2 >> buf)
      content += utf8Repair(buf) + " ";*/

    // to lowercase
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    if (prvUrl != url) {
      _urls.push_back(url);
      _records.push_back(content);
      ++docId;
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
