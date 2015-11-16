// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Anton Stepan <stepana@informatik.uni-freiburg.de>.

#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "./InvertedIndex.h"
#include "./QueryProcessor.h"
#include "./Posting.h"

using std::string;
using std::vector;
using std::stringstream;

// _____________________________________________________________________________
inline bool comparePostings(Posting i, Posting j) {
  return i.score > j.score;
}

// _____________________________________________________________________________
QueryProcessor::QueryProcessor(const InvertedIndex& ii) : _index(ii) { }

// _____________________________________________________________________________
vector<string> QueryProcessor::answerQuery(const string& query,
                                           const int k) {
  string word1 = "";
  string word2 = "";
  size_t pos = 0;
  pos = query.find(' ', 0);
  word1 = query.substr(0, pos);
  word2 = query.substr(pos+1);
  vector<Posting> resultDocumentIds;
  // getting all document Id's for given query
  if (word1 == word2) {
    resultDocumentIds = _index.getDocIdsForWord(word1);
  } else {
    resultDocumentIds = intersect(_index.getDocIdsForWord(word1),
                                  _index.getDocIdsForWord(word2));
  }
  std::partial_sort(resultDocumentIds.begin(),
                    resultDocumentIds.begin() + k,
                    resultDocumentIds.end(),
                    comparePostings);
  // generating output vector containing all result urls
  // and records
  vector<string> resultVector;
  auto end = resultDocumentIds.end();
  auto begin = resultDocumentIds.begin();
  size_t breakcount = 0;
  for (vector<Posting>::iterator it = begin; it != end; ++it) {
    size_t id = it->docId - 1;
    breakcount++;
    resultVector.push_back(_index.getUrl(id) + "\t" +
                           _index.getRecord(id));
    if (k == static_cast<int>(breakcount)) break;
  }
  return resultVector;
}

// _____________________________________________________________________________
vector<Posting> QueryProcessor::intersect(const vector<Posting>& list1,
                                          const vector<Posting>& list2) {
  vector<Posting> resultVector;
  size_t i = 0, j = 0;
  while ((i < list1.size()) && j < list2.size()) {
    if (list1[i].docId == list2[j].docId) {
      Posting tmpPosting(list1[i].docId, list1[i].score + list2[j].score);
      resultVector.push_back(tmpPosting);
      i++;
      j++;
    } else if (list1[i].docId > list2[j].docId) {
      j++;
    } else {
      i++;
    }
  }
  return resultVector;
}

// _____________________________________________________________________________
void QueryProcessor::prettyPrint(string resultLine, string word1,
                                 string word2) {
  string buf;
  vector<string> tokens;
  stringstream ss(resultLine);
  while (ss >> buf)
    tokens.push_back(buf);

  for (vector<string>::iterator it = tokens.begin();
     it != tokens.end(); ++it) {
    if (*it == word1 || *it == word2) {
      std:: cout << "\033[1m\033[31m" + (*it) + "\033[0m ";
    } else {
      std::cout << (*it) << " ";
    }
  }
  tokens.clear();
  std::cout << std::endl << std::endl;
}

// _____________________________________________________________________________
void QueryProcessor::print(string resultLine) {
  std::cout << resultLine << std::endl;
}
