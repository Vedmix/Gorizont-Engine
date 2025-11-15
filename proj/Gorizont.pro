TEMPLATE = app
TARGET = Gorizont
QT += widgets
CONFIG += c++14

# Исходные файлы
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

HEADERS += \
    headers/Camera.hpp \
    headers/Circle.hpp \
    headers/GameWindow.h \
    headers/MainMenuWindow.h \
    headers/Map.hpp \
    headers/Object2D.hpp \
    headers/ObjectType.hpp \
    headers/Point2D.hpp \
    headers/Polygon2D.hpp \
    headers/Wall.hpp \
    headers/World.hpp \
    headers/settings.hpp

# SFML
INCLUDEPATH += $$PWD/sfml/include \
               $$PWD/headers

# Кроссплатформенные настройки линковки
win32 {
    # Windows библиотеки
    LIBS += -L$$PWD/sfml/lib \
            -lsfml-graphics \
            -lsfml-window \
            -lsfml-system \
            -lopengl32 \
            -lgdi32 \
            -lwinmm
    
    # Автоматическое копирование DLL после сборки
    QMAKE_POST_LINK = $$QMAKE_POST_LINK $$quote(cp -f $$shell_path($$PWD/sfml/bin/*.dll) $$shell_path($$DESTDIR))
} else {
    # Linux библиотеки
    LIBS += -lsfml-graphics \
            -lsfml-window \
            -lsfml-system
            
    # Если SFML установлен в системных путях, убрать локальные пути
    unix:!macx {
        # Для системного SFML
        CONFIG += link_pkgconfig
        PKGCONFIG += sfml-all
    }
}

# Флаги компилятора
QMAKE_CXXFLAGS += -Wall -Wextra

# Папки для выходных файлов
DESTDIR = $$PWD/bin
OBJECTS_DIR = $$PWD/build
MOC_DIR = $$PWD/build/moc

# Настройки сборки
CONFIG(debug, debug|release) {
    TARGET = Gorizont_debug
    QMAKE_CXXFLAGS += -g
}

CONFIG(release, debug|release) {
    TARGET = Gorizont
    QMAKE_CXXFLAGS += -O2
}