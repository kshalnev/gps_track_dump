CXX = g++
CXXFLAGS = -g -std=c++11 -Wall -pedantic -O3
BIN = gps_track_dump

SRC = $(wildcard *.cpp)
OBJ = $(SRC:%.cpp=%.o)

all: $(OBJ)
	$(CXX) -o $(BIN) $^

%.o: %.c
	$(CXX) $@ -c $<

clean:
	rm -f *.o
	rm -f $(BIN)
