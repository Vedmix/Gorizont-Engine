# Универсальный Makefile для Gorizont-Engine
# Работает на всех Linux дистрибутивах

# Компилятор
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Iheaders

# Автодетект SFML
SFML_CFLAGS := $(shell pkg-config --cflags sfml-all 2>/dev/null || \
                pkg-config --cflags sfml-graphics 2>/dev/null || \
                echo "-I/usr/include/SFML -I/usr/local/include/SFML")

SFML_LIBS := $(shell pkg-config --libs sfml-all 2>/dev/null || \
              pkg-config --libs sfml-graphics sfml-window sfml-system 2>/dev/null || \
              echo "-lsfml-graphics -lsfml-window -lsfml-system")

# Автодетект Qt5
QT5_FOUND := $(shell pkg-config --exists Qt5Core Qt5Gui Qt5Widgets 2>/dev/null && echo 1 || echo 0)
ifeq ($(QT5_FOUND),1)
    QT_CFLAGS := $(shell pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets)
    QT_LIBS := $(shell pkg-config --libs Qt5Core Qt5Gui Qt5Widgets)
    MOC := $(shell which moc-qt5 2>/dev/null || which moc 2>/dev/null)
else
    # Проверяем пути вручную
    ifeq ($(wildcard /usr/include/qt5/QtCore),)
        QT_CFLAGS :=
        QT_LIBS :=
        MOC :=
    else
        QT_CFLAGS := -I/usr/include/qt5 -I/usr/include/qt5/QtCore \
                     -I/usr/include/qt5/QtGui -I/usr/include/qt5/QtWidgets
        QT_LIBS := -lQt5Core -lQt5Gui -lQt5Widgets
        MOC := $(shell which moc-qt5 2>/dev/null || which moc 2>/dev/null)
    endif
endif

# Системные библиотеки
SYS_LIBS := -lGL -lpthread -lX11 -ldl

# Финальные флаги
ALL_CFLAGS := $(CXXFLAGS) $(SFML_CFLAGS) $(QT_CFLAGS)
ALL_LIBS := $(SFML_LIBS) $(QT_LIBS) $(SYS_LIBS)

# Файлы проекта
SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(patsubst src/%.cpp,obj/%.o,$(SRC_FILES))
MOC_FILES :=

# Если есть Qt и moc, генерируем MOC файлы
ifneq ($(MOC),)
    MOC_FILES := moc/moc_GameWindow.cpp moc/moc_MainMenuWindow.cpp
    MOC_OBJ := $(patsubst %.cpp,%.o,$(MOC_FILES))
endif

# Цели
TARGET := Gorizont-Engine

# Директории ресурсов
SRC_FONTS_DIR := ../fonts
SRC_MAPS_DIR := ../maps
BUILD_FONTS_DIR := fonts
BUILD_MAPS_DIR := maps

.PHONY: all clean run install-deps info clean-build copy-resources

all: $(TARGET)

# Создание директорий
obj/%.o: src/%.cpp | obj
	@echo "Компиляция: $<"
	@$(CXX) $(ALL_CFLAGS) -c $< -o $@

# Создание MOC файлов
moc/moc_%.cpp: headers/%.hpp | moc
ifneq ($(MOC),)
	@echo "Генерация MOC: $<"
	@$(MOC) $< -o $@
else
	@echo "Ошибка: moc не найден для Qt файла $<"
	@false
endif

moc/%.o: moc/%.cpp
	@echo "Компиляция MOC: $<"
	@$(CXX) $(ALL_CFLAGS) -c $< -o $@

# Создание директорий сборки
obj:
	@mkdir -p obj

moc:
	@mkdir -p moc

# Линковка
$(TARGET): $(OBJ_FILES) $(MOC_OBJ)
	@echo "Линковка..."
	@$(CXX) $^ -o $@ $(ALL_LIBS)
	@echo "=== Сборка завершена! ==="
	@echo "Целевой файл: ./$(TARGET)"
	@echo "Размер: $$(du -h $(TARGET) | cut -f1)"

# Копирование ресурсов из исходных директорий в build
copy-resources:
	@echo "Копирование ресурсов..."
	@mkdir -p $(BUILD_FONTS_DIR) $(BUILD_MAPS_DIR)
	@if [ -d "$(SRC_FONTS_DIR)" ] && [ -n "$$(ls -A $(SRC_FONTS_DIR) 2>/dev/null)" ]; then \
		cp -r $(SRC_FONTS_DIR)/* $(BUILD_FONTS_DIR)/; \
		echo "Шрифты скопированы из $(SRC_FONTS_DIR)"; \
	else \
		echo "ВНИМАНИЕ: Исходные шрифты не найдены в $(SRC_FONTS_DIR)"; \
	fi
	@if [ -d "$(SRC_MAPS_DIR)" ] && [ -n "$$(ls -A $(SRC_MAPS_DIR) 2>/dev/null)" ]; then \
		cp -r $(SRC_MAPS_DIR)/* $(BUILD_MAPS_DIR)/; \
		echo "Карты скопированы из $(SRC_MAPS_DIR)"; \
	else \
		echo "ВНИМАНИЕ: Исходные карты не найдены в $(SRC_MAPS_DIR)"; \
	fi

# Проверка наличия ресурсов в build директории
check-resources:
	@echo "Проверка ресурсов в build..."
	@if [ ! -d "$(BUILD_FONTS_DIR)" ] || [ -z "$$(ls -A $(BUILD_FONTS_DIR) 2>/dev/null)" ]; then \
		echo "Шрифты в build не найдены. Выполняю copy-resources..."; \
		$(MAKE) copy-resources; \
	fi
	@if [ ! -d "$(BUILD_MAPS_DIR)" ] || [ -z "$$(ls -A $(BUILD_MAPS_DIR) 2>/dev/null)" ]; then \
		echo "Карты в build не найдены. Выполняю copy-resources..."; \
	fi

# Запуск
run: $(TARGET) check-resources
	@echo "Запуск $(TARGET)..."
	@./$(TARGET)

# Очистка только файлов сборки (без ресурсов)
clean-build:
	@rm -rf obj moc $(TARGET)
	@echo "Файлы сборки очищены"

# Полная очистка (включая скопированные ресурсы)
clean: clean-build
	@if [ -d "$(BUILD_FONTS_DIR)" ]; then \
		rm -rf $(BUILD_FONTS_DIR); \
		echo "Директория $(BUILD_FONTS_DIR) удалена"; \
	fi
	@if [ -d "$(BUILD_MAPS_DIR)" ]; then \
		rm -rf $(BUILD_MAPS_DIR); \
		echo "Директория $(BUILD_MAPS_DIR) удалена"; \
	fi
	@echo "Полная очистка завершена"

# Сборка с копированием ресурсов
build: $(TARGET) copy-resources

# Информация о сборке
info:
	@echo "=== Информация о сборке ==="
	@echo "Компилятор: $(CXX)"
	@echo "Qt5 найден: $(QT5_FOUND)"
	@echo "MOC: $(if $(MOC),$(MOC),не найден)"
	@echo "Исходные файлы: $(words $(SRC_FILES))"
	@echo "Цель: $(TARGET)"
	@echo "Исходные ресурсы:"
	@echo "  Шрифты: $(SRC_FONTS_DIR)"
	@echo "  Карты: $(SRC_MAPS_DIR)"
	@echo "  Собрано в: $(BUILD_FONTS_DIR), $(BUILD_MAPS_DIR)"
	@echo "Флаги: $(ALL_CFLAGS)"
	@echo "Библиотеки: $(ALL_LIBS)"

# Установка зависимостей
install-deps:
	@echo "=== Установка зависимостей ==="
	@echo "Пожалуйста, запустите команду для вашего дистрибутива:"
	@echo ""
	@echo "Для ALT Linux:"
	@echo "  sudo apt-get update && sudo apt-get install g++ libSFML-devel qt5-base-devel moc-qt5"
	@echo ""
	@echo "Для Ubuntu/Debian:"
	@echo "  sudo apt-get update && sudo apt-get install g++ libsfml-dev qt5-default qt5-qttools-dev"
	@echo ""
	@echo "Для Fedora:"
	@echo "  sudo dnf install gcc-c++ SFML-devel qt5-devel"
	@echo ""
	@echo "Для Arch Linux:"
	@echo "  sudo pacman -S gcc sfml qt5-base"