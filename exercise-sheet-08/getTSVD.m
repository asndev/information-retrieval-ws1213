% Copyright 2012, Anton Stepan <stepana@informatik.uni-freiburg.de>

function T = getTSVD(filename, k)
% getTSVD computes the truncated single value decomposition matrix.
% Usage: getTSVD(filename, k)
  tmp = load(filename);
  A = spconvert(tmp);
  clear tmp;

  [U, S, V] = svds(A, k);
  UU = U(:, 1:k);

  T = UU * UU';

end;
