#pragma once

#include "GameWindow.hpp"
#include "SettingsWindow.hpp"

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QMessageBox> // Добавлено для авторов
#include <QApplication> // Добавлено для выхода
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
    void onSettingsClosed();

private:
    QPushButton *playButton = nullptr;
    void initMenu();

    GameWindow* gameWindow;
    SettingsWindow* settingsWindow;
    const std::vector<QString> buttonNames = {
        "Играть",
        "Настройки",
        "Авторы",
        "Выход"
    };
};
