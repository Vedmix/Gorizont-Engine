#!/bin/bash
echo "Building Gorizont..."

# Сборка
make -j4

# Копирование DLL
cp -f sfml/bin/*.dll bin/

echo "Ready! Run: ./bin/Gorizont.exe"