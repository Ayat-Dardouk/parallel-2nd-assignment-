# Compiler and flags
CXX = mpicxx
CXXFLAGS = -std=c++11

# Program name
TARGET = prime_finder

# Source files
SRCS = mpiprimecyclic.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

