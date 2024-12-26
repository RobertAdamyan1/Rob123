
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
TARGET = prime-calculator

all: $(TARGET)

$(TARGET): prime-calculator.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) prime-calculator.cpp

clean:
	rm -f $(TARGET)
