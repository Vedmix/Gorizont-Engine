#Compiler/Compiler settings
CXX = g++
CXXFLAGS = -Wall -Wextra -Iheaders
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

#Project folders
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

#Files after compiling (without linking)
TARGET = $(BIN_DIR)/sfml-app

#Finding all the .cpp files in src dir
SRC = $(wildcard $(SRC_DIR)/*.cpp)
#Converting filenames .cpp to .o
OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

#Assembly
all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJ) -o $@ $(LIBS)
	@echo "Project successfully assembled to $(TARGET)"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Project cleaned"

.PHONY: all clean