CXX = g++ -O3 -Wall -std=c++0x
MAIN_BINARIES = $(basename $(wildcard *Main.cc))
TEST_BINARIES = $(basename $(wildcard *Test.cc))
HEADER = $(wildcard *.h)
OBJECTS = $(subst .cc,.o,$(filter-out %Main.cc %Test.cc,$(wildcard *.cc)))
MAIN_LIBS = -lboost_system
TEST_LIBS = -lgtest -lgtest_main -lpthread $(MAIN_LIBS)  

.PRECIOUS: %.o

all: compile test checkstyle

compile: $(MAIN_BINARIES) $(TEST_BINARIES)

test: $(TEST_BINARIES)
	for T in $(TEST_BINARIES); do ./$$T; done

checkstyle:
	python ../cpplint.py *.h *.cc

clean:
	rm -f *.o
	rm -f $(MAIN_BINARIES)
	rm -f $(TEST_BINARIES)
	rm -f *.class
	rm -f *Test.TMP.*
	rm -f core

%Main: %Main.o $(OBJECTS)
	$(CXX) -o $@ $^ $(MAIN_LIBS) 

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@ $^ $(TEST_LIBS)

%.o: %.cc $(HEADER)
	$(CXX) -c $<
