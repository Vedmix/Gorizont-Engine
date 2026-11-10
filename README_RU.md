# Gorizont-Engine

[English version](README.md)

## Как запустить 

**1.** Для запуска проекта вам следует установить следующие пакеты

Qt:
```
sudo apt install build-essential qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtcreator
```
SFML:
```
wget https://www.sfml-dev.org/files/SFML-3.1.0-linux-gcc-64-bit.tar.gz
```

**2.** Чтобы собрать исполняемый файл вам нужно:
(Исполнять в корневой папке)
```
qmake Gorizont.pro
make
```

**3.** Запуск производить из корневой папки с учетом вашей графической системы:

X11:
```
./bin/Gorizont-Engine
```

Остальные:
```
QT_QPA_PLATFORM=xcb ./bin/Gorizont-Engine
```