CXX = g++ -O3 -Wall -std=c++0x
MAIN_BINARIES = $(basename $(wildcard *Main.cc))
TEST_BINARIES = $(basename $(wildcard *Test.cc))
HEADER = $(wildcard *.h)
OBJECTS = $(addsuffix .o, $(basename $(filter-out %Main.cc %Test.cc,$(wildcard *.cc)))) 

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
	$(CXX) -o $@ $^

%Test: %Test.o $(OBJECTS)
	$(CXX) -o $@ $^ -lgtest -lgtest_main -lpthread

%.o: %.cc $(HEADER)
	$(CXX) -c $<
