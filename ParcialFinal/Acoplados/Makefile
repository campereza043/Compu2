CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -O2

SRC = src/main_acoplado.cpp src/coupled_simulator.cpp
OBJS = $(SRC:.cpp=.o)

TARGET = bin/motor_acoplado

all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET) src/*.o bin/*

.PHONY: all clean
