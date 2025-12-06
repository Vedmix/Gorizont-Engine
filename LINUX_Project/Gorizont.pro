QT += core gui widgets

TARGET = Gorizont-Engine
TEMPLATE = app
CONFIG += c++17

# Исходные файлы (явное перечисление)
SOURCES += \
    src/main.cpp \
    src/GameWindow.cpp \
    src/MainMenuWindow.cpp \
    src/Camera.cpp \
    src/Circle.cpp \
    src/Map.cpp \
    src/Object2D.cpp \
    src/Point2D.cpp \
    src/Polygon2D.cpp \
    src/Wall.cpp \
    src/World.cpp

HEADERS += \
    headers/GameWindow.hpp \
    headers/MainMenuWindow.hpp \
    headers/Camera.hpp \
    headers/Circle.hpp \
    headers/Map.hpp \
    headers/Object2D.hpp \
    headers/Point2D.hpp \
    headers/Polygon2D.hpp \
    headers/settings.hpp \
    headers/Wall.hpp \
    headers/World.hpp

INCLUDEPATH += headers

# Для Linux (ALT Linux)
linux {
    QMAKE_CXXFLAGS += -Wall -Wextra -std=c++17

    # Пути к заголовкам SFML
    INCLUDEPATH += /usr/include/SFML

    # Только ОСНОВНЫЕ библиотеки SFML (в ALT Linux: libsfml-*)
    LIBS += -lsfml-graphics
    LIBS += -lsfml-window
    LIBS += -lsfml-system

    # Минимальный набор системных библиотек
    LIBS += -lGL
    LIBS += -lpthread
    LIBS += -lX11
}

DISTFILES += \
    fonts/font.ttf \
    maps/map1.xml \
    maps/map2.xml
