-- Problem Set 12
-- Exercise 1
-- Anton Stepan <stepana@informatik.uni-freiburg.de>
--
create table acted_in (subject TEXT, object TEXT);
create table married_to (subject TEXT, object TEXT);
create table has_won (subject TEXT, object TEXT);

-- .import freebase.married_to.tsv married_to
-- .import freebase.has_won.tsv has_won
-- .import freebase.acted_in.tsv acted_in

