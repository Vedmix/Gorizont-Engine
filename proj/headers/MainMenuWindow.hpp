#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <vector>
#include <QString>
#include "../headers/GameWindow.hpp" // Полный include вместо forward declaration
#include "../headers/settings.hpp" // Добавляем для SCREEN_WIDTH и SCREEN_HEIGHT

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
    void initMenu();

    GameWindow* gameWindow;
    const std::vector<QString> buttonNames = {
        "Играть",
        "Настройки",
        "Об игре",
        "Выход"
    };
};
