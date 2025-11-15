# Компилятор и флаги
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -g -Isfml/include -Iheaders
LIBS = -Lsfml/lib -lsfml-graphics -lsfml-window -lsfml-system -lopengl32 -lgdi32 -lwinmm -lQt5Widgets -lQt5Gui -lQt5Core

# Автопоиск исходников
SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(SOURCES:src/%.cpp=build/%.o)
TARGET = bin/Gorizont.exe

# Основная цель
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LIBS)

# Компиляция объектных файлов
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Вспомогательные цели
.PHONY: all run clean

all: $(TARGET)

run: all
	cp -f sfml/bin/*.dll bin/
	./$(TARGET)

clean:
	rm -rf build/*.o $(TARGET)

distclean: clean
	rm -f .qmake.stash
	rm -rf .qtcreator/