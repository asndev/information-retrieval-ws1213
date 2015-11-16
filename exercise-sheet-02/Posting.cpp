// Copyright 2012, University of Freiburg,
// Chair of Algorithms and Data Structures.
// Author: Hannah Bast <bast@informatik.uni-freiburg.de>.
// modified by Anton Stepan <stepana@informatik.uni-freiburg.de>

#include <algorithm>
#include <vector>
#include "./Posting.h"

using std::vector;

// _____________________________________________________________________________
Posting::Posting(int id, float scr) {
  docId = id;
  score = scr;
}

// ____________________________________________________________________________
bool operator== (const Posting &p1, const Posting &p2) {
  return (p1.docId == p2.docId && p1.score == p2.score);
}

// ____________________________________________________________________________
bool operator!= (const Posting &p1, const Posting p2) {
  return !(p1 == p2);
}

// ____________________________________________________________________________
bool Posting::operator<(const Posting& p) const {
  return score < p.score;
}
