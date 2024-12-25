CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = signal-echo

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.cpp

clean:
	rm -f $(TARGET)
