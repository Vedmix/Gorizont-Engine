# Вместо QT -= core gui добавьте необходимые модули Qt
QT += core gui widgets

TARGET = Gorizont-Engine
TEMPLATE = app
CONFIG += c++17

# Источники
SOURCES += \
    src/GameWindow.cpp \
    src/MainMenuWindow.cpp \
    src/main.cpp \
    src/Camera.cpp \
    src/Circle.cpp \
    src/Map.cpp \
    src/Object2D.cpp \
    src/Point2D.cpp \
    src/Polygon2D.cpp \
    src/Wall.cpp \
    src/World.cpp

# Заголовки
HEADERS += \
    headers/GameWindow.hpp \
    headers/Camera.hpp \
    headers/Circle.hpp \
    headers/MainMenuWindow.hpp \
    headers/Map.hpp \
    headers/Object2D.hpp \
    headers/Point2D.hpp \
    headers/Polygon2D.hpp \
    headers/settings.hpp \
    headers/Wall.hpp \
    headers/World.hpp

# Пути включения
INCLUDEPATH += $$PWD/headers \
               $$PWD/sfml/include

# Настройки компилятора
QMAKE_CXXFLAGS += -Wall -Wextra -DSFML_STATIC

# Библиотеки для статической линковки SFML
LIBS += -L$$PWD/sfml/lib
LIBS += -lsfml-graphics-s
LIBS += -lsfml-window-s
LIBS += -lsfml-system-s
LIBS += -lopengl32
LIBS += -lfreetype
LIBS += -lwinmm
LIBS += -lgdi32

# Для статической линковки стандартных библиотек
QMAKE_LFLAGS += -static-libgcc -static-libstdc++
