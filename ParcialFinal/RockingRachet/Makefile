CXX = g++
CXXFLAGS = -Wall -O2 -std=c++17 -Iinclude
SRCDIR = src
OBJDIR = obj
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

TARGET = $(BINDIR)/motor_ratcheting

.PHONY: all clean dirs run

all: dirs $(TARGET)

dirs:
	mkdir -p $(OBJDIR) $(BINDIR) results

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJDIR) $(BINDIR) results/trajectory.dat

run: all
	./$(TARGET)

analizar:
	python3 scripts/analizar_resultados.py results/trajectory.dat

energia:
	python3 scripts/energia.py results/trajectory.dat
animar:
	python3 scripts/animar.py results/trajectory.dat