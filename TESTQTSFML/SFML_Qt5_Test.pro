TEMPLATE = app
TARGET = sfml_qt_test
CONFIG += c++14 debug
QT += widgets

SOURCES += main.cpp main_window.cpp sfml_widget.cpp
HEADERS += main_window.h sfml_widget.h

QMAKE_CXXFLAGS += -Wall -Wextra

# Используем release версии библиотек
LIBS += -lsfml-graphics -lsfml-window -lsfml-system

linux {
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtWidgets
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtGui
    INCLUDEPATH += /usr/include/x86_64-linux-gnu/qt5/QtCore
}