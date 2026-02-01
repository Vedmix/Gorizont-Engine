TARGET = Gorizont
CXX = g++
MOC = moc-qt5

INC = -Iheaders -Isfml/include
QT_INC = -Iqt_local/include -Iqt_local/include/QtWidgets -Iqt_local/include/QtGui -Iqt_local/include/QtCore
LIB = -Lsfml/lib -Lqt_local/lib
RPATH = -Wl,-rpath,sfml/lib:qt_local/lib

CXXFLAGS = -Wall -std=c++17 -fPIC $(INC) $(QT_INC)
LDFLAGS = $(LIB) $(RPATH) -lsfml-graphics -lsfml-window -lsfml-system -lQt5Widgets -lQt5Gui -lQt5Core -lpthread

SRC = $(shell find src -name "*.cpp")
MOC_SRC = $(wildcard moc/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o) $(MOC_SRC:moc/%.cpp=build/moc/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/moc/%.o: moc/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGET) build/

