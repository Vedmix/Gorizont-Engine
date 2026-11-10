# Gorizont-Engine

[Русская версия](README_RU.md)

## How to run

**1.** To run the project, you need to install the following packages

Qt:
```
sudo apt install build-essential qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtcreator
```
SFML:
```
wget https://www.sfml-dev.org/files/SFML-3.1.0-linux-gcc-64-bit.tar.gz
```

**2.** To build the executable, you need to:
(Run from the project root directory)
```
qmake Gorizont.pro
make
```

**3.** Run from the project root directory, depending on your graphics system:

X11:
```
./bin/Gorizont-Engine
```

Others:
```
QT_QPA_PLATFORM=xcb ./bin/Gorizont-Engine
```