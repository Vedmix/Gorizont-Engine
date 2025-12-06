# Универсальный Makefile для Gorizont-Engine
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iheaders -fPIC

# SFML
SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics 2>/dev/null || echo "-I/usr/include/SFML")
SFML_LIBS := $(shell pkg-config --libs sfml-graphics sfml-window sfml-system 2>/dev/null || echo "-lsfml-graphics -lsfml-window -lsfml-system")

# Qt
QT_CFLAGS := $(shell pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets 2>/dev/null || echo "-I/usr/include/qt5 -I/usr/include/qt5/QtCore -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtWidgets")
QT_LIBS := $(shell pkg-config --libs Qt5Core Qt5Gui Qt5Widgets 2>/dev/null || echo "-lQt5Core -lQt5Gui -lQt5Widgets")

# Проверяем moc
MOC := $(shell which moc-qt5 2>/dev/null || which moc 2>/dev/null)

ALL_CFLAGS := $(CXXFLAGS) $(SFML_CFLAGS) $(QT_CFLAGS)
ALL_LIBS := $(QT_LIBS) $(SFML_LIBS) -lGL -lpthread -lX11 -ldl

# Исходные файлы
SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(patsubst src/%.cpp,obj/%.o,$(SRC_FILES))

# Qt заголовки, требующие moc
QT_HEADERS := headers/GameWindow.hpp headers/MainMenuWindow.hpp
MOC_FILES := $(patsubst headers/%.hpp,moc/%.moc.cpp,$(QT_HEADERS))
MOC_OBJ_FILES := $(patsubst headers/%.hpp,obj/moc_%.o,$(QT_HEADERS))

TARGET := Gorizont-Engine

# Директории ресурсов
SRC_FONTS_DIR := ../fonts
SRC_MAPS_DIR := ../maps
BUILD_FONTS_DIR := fonts
BUILD_MAPS_DIR := maps

.PHONY: all clean run run-xcb run-wayland moc

all: $(TARGET)

# Компиляция обычных .cpp файлов
obj/%.o: src/%.cpp | obj
	@mkdir -p obj
	@echo "Компиляция: $<"
	@$(CXX) $(ALL_CFLAGS) -c $< -o $@

# Генерация moc файлов
moc/%.moc.cpp: headers/%.hpp | moc
ifneq ($(MOC),)
	@echo "Генерация MOC для: $<"
	@$(MOC) $< -o $@
else
	@echo "ОШИБКА: moc не найден!"
	@echo "Установите moc: sudo apt install qt5-qttools-dev"
	@false
endif

# Компиляция moc файлов
obj/moc_%.o: moc/%.moc.cpp | obj
	@echo "Компиляция MOC: $<"
	@$(CXX) $(ALL_CFLAGS) -c $< -o $@

# Создание директорий
obj:
	@mkdir -p obj

moc:
	@mkdir -p moc

# Линковка ВСЕГО
$(TARGET): $(OBJ_FILES) $(MOC_OBJ_FILES)
	@echo "Линковка..."
	@$(CXX) -fPIE $^ -o $@ $(ALL_LIBS)
	@echo "=== Сборка завершена! ==="
	@echo "Целевой файл: ./$(TARGET)"
	@echo "Размер: $$(du -h $(TARGET) | cut -f1)"

# Цель только для генерации moc файлов
moc-files: $(MOC_FILES)
	@echo "MOC файлы сгенерированы"

# Копирование ресурсов
copy-resources:
	@mkdir -p $(BUILD_FONTS_DIR) $(BUILD_MAPS_DIR)
	@if [ -d "$(SRC_FONTS_DIR)" ] && [ -n "$$(ls -A $(SRC_FONTS_DIR) 2>/dev/null)" ]; then \
		cp -r $(SRC_FONTS_DIR)/* $(BUILD_FONTS_DIR)/; \
		echo "Шрифты скопированы"; \
	else \
		echo "ВНИМАНИЕ: Исходные шрифты не найдены"; \
	fi
	@if [ -d "$(SRC_MAPS_DIR)" ] && [ -n "$$(ls -A $(SRC_MAPS_DIR) 2>/dev/null)" ]; then \
		cp -r $(SRC_MAPS_DIR)/* $(BUILD_MAPS_DIR)/; \
		echo "Карты скопированы"; \
	else \
		echo "ВНИМАНИЕ: Исходные карты не найдены"; \
	fi

check-resources:
	@echo "Проверка ресурсов..."
	@if [ ! -d "$(BUILD_FONTS_DIR)" ] || [ -z "$$(ls -A $(BUILD_FONTS_DIR) 2>/dev/null)" ]; then \
		echo "Шрифты не найдены, копирую..."; \
		$(MAKE) copy-resources; \
	fi
	@if [ ! -d "$(BUILD_MAPS_DIR)" ] || [ -z "$$(ls -A $(BUILD_MAPS_DIR) 2>/dev/null)" ]; then \
		echo "Карты не найдены, копирую..."; \
	fi

# Запуск
run-xcb: $(TARGET) check-resources
	@echo "Запуск с XCB (X11)..."
	@QT_QPA_PLATFORM=xcb ./$(TARGET)

run: run-xcb

# Очистка
clean:
	@rm -rf obj moc $(TARGET) $(BUILD_FONTS_DIR) $(BUILD_MAPS_DIR)
	@echo "Очистка завершена"

# Информация
info:
	@echo "=== Информация о сборке ==="
	@echo "Компилятор: $(CXX)"
	@echo "MOC: $(if $(MOC),$(MOC),не найден)"
	@echo "Qt заголовки: $(QT_HEADERS)"
	@echo "MOC файлы: $(MOC_FILES)"
	@echo "Флаги: $(ALL_CFLAGS)"
	@echo "Библиотеки: $(ALL_LIBS)"

# Установка зависимостей
install-deps:
	@echo "Установка зависимостей..."
	@echo "Для Ubuntu/Debian:"
	@echo "sudo apt-get update && sudo apt-get install g++ libsfml-dev qt5-default qt5-qttools-dev"
	@echo ""
	@echo "Для ALT Linux:"
	@echo "sudo apt-get update && sudo apt-get install g++ libSFML-devel qt5-base-devel moc-qt5"