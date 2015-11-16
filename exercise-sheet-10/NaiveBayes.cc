// Copyright 2012
// Anton Stepan <stepana@informatik.uni-freiburg.de>

#include "./NaiveBayes.h"
#include <unordered_map>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <set>
#include <string>
#include <vector>
#include "./Intersect.h"

using std::set;
using std::string;
using std::vector;

const int MINWORDLENGTH = 3;

// _____________________________________________________________________________
void NaiveBayes::train(vector<int> DocumentIds) {
  assert(_documentClasses.size() == _documentVectors.size());
  size_t docSize = _documentClasses.size();
  std::unordered_map<int, int> m;
  for (size_t i = 0; i < docSize; ++i) {
    m[_documentClasses[i][0]] += _documentVectors[i].size();
    /*if (m.count(_documentClasses[i][0])) {
      m[_documentClasses[i][0]] =
      Intersect::IntersectV2(m[_documentClasses[i][0]], _documentVectors[i]);
    } else {
      m[_documentClasses[i][0]] = _documentVectors[i];
    }*/
  }
  int allClassesSize = 0;
  size_t mapSize = m.size();
  for (size_t i = 0; i < mapSize; ++i) {
    // std::sort(m.at(i).begin(), m.at(i).end());
    // m.at(i).erase(std::unique(m.at(i).begin(), m.at(i).end()), m.at(i).end());
    allClassesSize += m.at(i);
  }
  // fill the P(C=c) vector.
  for (size_t i = 0; i < _classes.size(); ++i) {
    _pc.push_back((m.at(i) + 1.0) / (allClassesSize + 1.0));
    std::cout << _pc[i] << std::endl;
  }
}

// _____________________________________________________________________________
void NaiveBayes::ReadDocumentsAndLabels(string csvFileName) {
  using std::cout;
  using std::endl;
  using std::ifstream;

  ifstream file(csvFileName.c_str());
  string line;
  int docId = 0;
  while (true) {
    getline(file, line);
    if (file.eof()) break;
    size_t pos = 0, pos2 = 0;
    pos = line.find('\t', pos);
    pos2 = line.find('\t', pos+1);
    // line in wrong format?
    if (pos == string::npos || pos2 == string::npos) perror("Wrong format");
    assert(pos != pos2);
    string url =  line.substr(0, pos);
    string sClass = line.substr(pos+1, pos2-pos-1);
    string content = line.substr(pos2+1);
    ++docId;
    // to lowercase
    std::transform(line.begin(), line.end(), line.begin(), ::tolower);

    // check if class already exists
    int classId = getClassId(sClass);
    // add Class to docid
    vector<int> tmpSet;
    tmpSet.push_back(classId);
    _documentClasses.push_back(tmpSet);
    // Parse the line and add each word to the respective index list.
    const size_t linesize = line.size();
    vector<int> wordsInDoc;
    while (pos2 < linesize) {
      while (pos2 < linesize && !isalpha(line[pos2])) pos2++;
      size_t wordStart = pos2;
      while (pos2 < linesize && isalpha(line[pos2])) pos2++;
      size_t wordEnd = pos2;
      if (wordEnd >= wordStart + MINWORDLENGTH) {
        string word = line.substr(wordStart, wordEnd - wordStart);
        wordsInDoc.push_back(getWordId(word));
      }
    }
    _documentVectors.push_back(wordsInDoc);
  }
}

// _____________________________________________________________________________
int NaiveBayes::getClassId(const string& s) {
  auto it = std::find(_classes.begin(), _classes.end(), s);
  int res = -1;
  if (it != _classes.end()) {
    res = std::distance(_classes.begin(), it);
  } else {
    _classes.push_back(s);
    res = _classes.size() - 1;
  }
  assert(res != -1);
  return res;
}

// _____________________________________________________________________________
int NaiveBayes::getWordId(const string& s) {
  auto it = std::find(_words.begin(), _words.end(), s);
  int res = -1;
  if (it != _words.end()) {
    res = std::distance(_words.begin(), it);
  } else {
    _words.push_back(s);
    res = _words.size() - 1;
  }
  assert(res != -1);
  return res;
}
