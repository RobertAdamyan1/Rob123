CXX = g++
CXXFLAGS = -std=c++17 -Wall -pthread -lrt

SRCS = shared-array.cpp first.cpp second.cpp
OBJS = shared-array.o first.o second.o
EXEC1 = first
EXEC2 = second

all: $(EXEC1) $(EXEC2)

$(EXEC1): shared-array.o first.o
	$(CXX) $(CXXFLAGS) -o $(EXEC1) shared-array.o first.o

$(EXEC2): shared-array.o second.o
	$(CXX) $(CXXFLAGS) -o $(EXEC2) shared-array.o second.o

shared-array.o: shared-array.cpp shared-array.h
	$(CXX) $(CXXFLAGS) -c shared-array.cpp

first.o: first.cpp shared-array.h
	$(CXX) $(CXXFLAGS) -c first.cpp

second.o: second.cpp shared-array.h
	$(CXX) $(CXXFLAGS) -c second.cpp

clean:
	rm -f $(OBJS) $(EXEC1) $(EXEC2)
