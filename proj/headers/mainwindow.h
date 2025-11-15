#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "World.hpp"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QTimer>
#include <QButtonGroup>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QStringList buttonNames = {
        "Играть",
        "Настройки",
        "Авторы",
        "Выход"
    };

public:
    MainWindow(QWidget *parent = nullptr, int choice = 0);
    ~MainWindow();

private slots:
    void initMenu();
    void openSfmlWindow();
    void handleButton(int id);

};
#endif // MAINWINDOW_H
