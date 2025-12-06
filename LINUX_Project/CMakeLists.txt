cmake_minimum_required(VERSION 3.10)
project(Gorizont-Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Ищем Qt5 по-старинке
find_package(Qt5 COMPONENTS Core Gui Widgets QUIET)

if(NOT Qt5_FOUND)
    message(WARNING "Qt5 не найден через find_package, используем ручные настройки")
    
    # Ручные настройки для ALT Linux
    set(QT5_INCLUDE_DIRS
        /usr/include/qt5
        /usr/include/qt5/QtCore
        /usr/include/qt5/QtGui
        /usr/include/qt5/QtWidgets
    )
    
    set(QT5_LIBRARIES
        Qt5Core
        Qt5Gui
        Qt5Widgets
    )
    
    # Проверяем существование
    foreach(dir ${QT5_INCLUDE_DIRS})
        if(EXISTS ${dir})
            message(STATUS "Найден Qt5 путь: ${dir}")
        else()
            message(WARNING "Путь Qt5 не существует: ${dir}")
        endif()
    endforeach()
else()
    message(STATUS "Qt5 найден: ${Qt5_VERSION}")
    set(QT5_INCLUDE_DIRS ${Qt5_INCLUDE_DIRS})
    set(QT5_LIBRARIES Qt5::Core Qt5::Gui Qt5::Widgets)
endif()

# Исходные файлы
set(SOURCES
    src/main.cpp
    src/GameWindow.cpp
    src/MainMenuWindow.cpp
    src/Camera.cpp
    src/Circle.cpp
    src/Map.cpp
    src/Object2D.cpp
    src/Point2D.cpp
    src/Polygon2D.cpp
    src/Wall.cpp
    src/World.cpp
)

set(HEADERS
    headers/GameWindow.hpp
    headers/MainMenuWindow.hpp
    headers/Camera.hpp
    headers/Circle.hpp
    headers/Map.hpp
    headers/Object2D.hpp
    headers/Point2D.hpp
    headers/Polygon2D.hpp
    headers/settings.hpp
    headers/Wall.hpp
    headers/World.hpp
)

# Исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Включение директорий
target_include_directories(${PROJECT_NAME} PRIVATE
    headers
    /usr/include/SFML
    ${QT5_INCLUDE_DIRS}
)

# Линковка библиотек
target_link_libraries(${PROJECT_NAME}
    sfml-graphics
    sfml-window
    sfml-system
    ${QT5_LIBRARIES}
    GL
    pthread
    X11
    dl
)

# MOC вручную (если нужно)
if(Qt5_FOUND)
    # Автоматический MOC если Qt найден
    set_target_properties(${PROJECT_NAME} PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )
else()
    # Ручной MOC
    find_program(MOC_EXECUTABLE moc-qt5 moc)
    if(MOC_EXECUTABLE)
        message(STATUS "Найден moc: ${MOC_EXECUTABLE}")
        # Генерируем MOC файлы
        add_custom_command(
            TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND ${MOC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/headers/GameWindow.hpp -o ${CMAKE_CURRENT_BINARY_DIR}/moc_GameWindow.cpp
            COMMAND ${MOC_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/headers/MainMenuWindow.hpp -o ${CMAKE_CURRENT_BINARY_DIR}/moc_MainMenuWindow.cpp
            COMMENT "Генерация MOC файлов..."
        )
        target_sources(${PROJECT_NAME} PRIVATE
            ${CMAKE_CURRENT_BINARY_DIR}/moc_GameWindow.cpp
            ${CMAKE_CURRENT_BINARY_DIR}/moc_MainMenuWindow.cpp
        )
    else()
        message(WARNING "MOC не найден, MOC файлы не сгенерированы")
    endif()
endif()