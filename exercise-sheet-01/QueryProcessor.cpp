// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Anton Stepan <stepana@informatik.uni-freiburg.de>.

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "./QueryProcessor.h"

using std::string;
using std::vector;
using std::stringstream;

// _____________________________________________________________________________
QueryProcessor::QueryProcessor() {
}

// _____________________________________________________________________________
QueryProcessor::QueryProcessor(const InvertedIndex& ii) {
  _index = ii;
}

// _____________________________________________________________________________
vector<string> QueryProcessor::answerQuery(string query, int k) {
  string word1 = "";
  string word2 = "";
  size_t pos = 0;
  pos = query.find(' ', 0);
  word1 = query.substr(0, pos);
  word2 = query.substr(pos+1);
  vector<int> resultDocumentIds;
  // getting all document Id's for given query
  if (word1 == word2) {
    resultDocumentIds = answerSingleQuery(word1);
  } else {
    resultDocumentIds = intersect(answerSingleQuery(word1),
                                            answerSingleQuery(word2));
  }
  // generating output vector containing all result urls
  // and records
  vector<string> resultVector;
  int breakCount = 0;
  for (vector<int>::iterator it = resultDocumentIds.begin();
       it != resultDocumentIds.end(); ++it) {
    resultVector.push_back(_index.getUrlAt((*it)-1) + "\t" +
                           _index.getRecordAt((*it)-1));
    breakCount++;
    // return only as many results as specified by the user
    if (breakCount == k) break;
  }
  return resultVector;
}

// _____________________________________________________________________________
vector<int> QueryProcessor::answerSingleQuery(string word) {
  vector<int> v = _index.getDocIdsForWord(word);
  return v;
}

// _____________________________________________________________________________
vector<int> QueryProcessor::intersect(vector<int> list1, vector<int> list2) {
  vector<int> resultVector;
  size_t i = 0, j = 0;
  while ((i < list1.size()) && (j < list2.size())) {
    if (list1[i] == list2[j]) {
      resultVector.push_back(list1[i]);
      i++;
      j++;
    } else if (list1[i] > list2[j]) {
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
  std::cout << std::endl;
}

// _____________________________________________________________________________
void QueryProcessor::print(string resultLine) {
  std::cout << resultLine << std::endl;
}
