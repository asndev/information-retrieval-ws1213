// Copyright 2012, University of Freiburg,
// Anton Stepan <stepana@informatik.uni-freiburg.de>

#ifndef EXERCISE_SHEET_08_POSTING_H_
#define EXERCISE_SHEET_08_POSTING_H_

#include <gtest/gtest.h>
#include <string>
#include <vector>

using std::string;
using std::vector;

// A Posting is a pair of a document id and a score.
class Posting {
 public:
  explicit Posting(int id, float scr);
  int docId;
  float score;

  void setScore(float scr);

  friend bool operator== (const Posting &p1, const Posting &p2);
  friend bool operator!= (const Posting &p1, const Posting &p2);
  bool operator<(const Posting& p) const;

  friend std::ostream & operator<<(std::ostream & _stream,
                                   Posting const & post) {
    return _stream << "( id:" <<  post.docId << ' '
                   << " score:" << post.score << " )" << std::endl;
  }
};

#endif  // EXERCISE_SHEET_08_POSTING_H_
