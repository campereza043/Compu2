CXX = g++
CXXFLAGS = -std=c++17 -O2 -Iinclude -Wall -Wextra
SRC = src
OBJ = obj
BIN = bin
TARGET = $(BIN)/generador_gauss

SRCS = $(SRC)/RandomEngine.cpp $(SRC)/GeneradorGauss.cpp $(SRC)/main.cpp
OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))

.PHONY: all clean run plot

all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJ)/%.o: $(SRC)/%.cpp | $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

$(BIN):
	mkdir -p $(BIN)

run: all
	./$(TARGET)

plot:
	@echo "Creando carpeta results/ si no existe..."
	mkdir -p results
	@echo "Ejecutando gnuplot script..."
	gnuplot scripts/plot_gauss.gp
	@echo "Figura generada en results/hist_gauss.png"

clean:
	rm -rf $(OBJ) $(BIN) results/*.dat results/*.png
