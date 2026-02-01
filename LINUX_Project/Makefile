TARGET = Gorizont
CXX = g++
MOC = moc-qt5

# Пути
QT_INC = qt_local/include
SFML_INC = sfml/include
HEADERS = headers
MOC_DIR = moc

# Флаги компиляции
CXXFLAGS = -Wall -std=c++17 \
           -I$(HEADERS) \
           -I$(SFML_INC) \
           -I$(QT_INC) \
           -I$(QT_INC)/QtWidgets \
           -I$(QT_INC)/QtGui \
           -I$(QT_INC)/QtCore \
           -fPIC

# Флаги компоновщика
QT_LIB = qt_local/lib
SFML_LIB = sfml/lib
LDFLAGS = -L$(SFML_LIB) -L$(QT_LIB) \
          -Wl,-rpath,$(SFML_LIB):$(QT_LIB) \
          -lsfml-graphics -lsfml-window -lsfml-system \
          -lQt5Widgets -lQt5Gui -lQt5Core -lpthread

# Исходные файлы
CPP_SRCS = $(shell find src -name "*.cpp")
MOC_SRCS = $(wildcard $(MOC_DIR)/*.cpp)

# Объектные файлы
CPP_OBJS = $(CPP_SRCS:src/%.cpp=build/%.o)
MOC_OBJS = $(MOC_SRCS:$(MOC_DIR)/%.cpp=build/moc/%.o)
OBJS = $(CPP_OBJS) $(MOC_OBJS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

# Правило для обычных .cpp файлов
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Правило для MOC .cpp файлов
build/moc/%.o: $(MOC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Очистка
clean:
	rm -rf $(TARGET) build/ $(MOC_DIR)/*.o $(MOC_DIR)/*.moc.cpp 2>/dev/null || true

# Перегенерация MOC файлов (опционально)
moc-clean:
	rm -f $(MOC_DIR)/*.cpp

moc-generate: moc-clean
	@mkdir -p $(MOC_DIR)
	$(MOC) -I$(HEADERS) -I$(QT_INC) headers/GameWindow.hpp -o $(MOC_DIR)/GameWindow.moc.cpp
	$(MOC) -I$(HEADERS) -I$(QT_INC) headers/MainMenuWindow.hpp -o $(MOC_DIR)/MainMenuWindow.moc.cpp

.PHONY: all clean moc-clean moc-generate