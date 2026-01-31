TARGET = Gorizont
CXX = g++
CXXFLAGS = -Wall -std=c++17 -Iheaders -Isfml/include
LDFLAGS = -Lsfml/lib -Wl,-rpath,sfml/lib -lsfml-graphics -lsfml-window -lsfml-system

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:src/%.cpp=build/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

build/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run