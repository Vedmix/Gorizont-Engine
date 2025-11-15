TEMPLATE = app
TARGET = sfml_qt_test
CONFIG += c++14
QT += widgets

SOURCES += main.cpp main_window.cpp sfml_widget.cpp
HEADERS += main_window.h sfml_widget.h

# Пути к SFML 2.6.2
INCLUDEPATH += "C:/Users/vedmix/SFML-2.6.2/include"
LIBS += -L"C:/Users/vedmix/SFML-2.6.2/lib" \
        -lsfml-graphics \
        -lsfml-window \
        -lsfml-system

# Системные библиотеки Windows
LIBS += -lopengl32 -lgdi32 -lwinmm

# Минимальные флаги
QMAKE_CXXFLAGS += -Wall