CXX = g++
CXXFLAGS = -Wall -I./include
LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_ttf
SRC_DIR = ./src
OBJ_DIR = ./obj
BIN_DIR = ./bin


SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/program: $(OBJ)
	$(CXX) $(LDFLAGS) $^ -o $@ 

.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/program
	rm -f $(OBJ) $(EXECUTABLE)
