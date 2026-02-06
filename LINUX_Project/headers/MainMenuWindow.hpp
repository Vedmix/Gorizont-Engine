#pragma once

#include "GameWindow.hpp"

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <vector>
#include <QString>

class MainMenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenuWindow(QWidget* parent = nullptr, int choice = 0);
    ~MainMenuWindow();

private slots:
    void handleButton(int id);
    void onGameFinished();

private:
    QPushButton *playButton = nullptr;
    void initMenu();

    GameWindow* gameWindow;
    const std::vector<QString> buttonNames = {
        "Играть",
        "Настройки",
        "Авторы",
        "Выход"
    };
};
