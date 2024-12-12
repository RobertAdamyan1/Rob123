CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

TARGET = interactive-shell

all: $(TARGET)

$(TARGET): interactive-shell.cpp
 $(CXX) $(CXXFLAGS) -o $(TARGET) interactive-shell.cpp

clean:
 rm -f $(TARGET) *.log
