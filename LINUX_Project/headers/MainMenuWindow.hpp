#pragma once

#include <GameWindow.hpp>
#include <SettingsWindow.hpp>
#include <CreditsWindow.hpp>
#include <AppSettings.hpp>

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
    void onSettingsClosed();
    void onCreditsClosed();
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    QPushButton *playButton = nullptr;
    void initMenu();

    GameWindow* gameWindow;
    SettingsWindow* settingsWindow;
    CreditsWindow* creditsWindow;

    const std::vector<QString> buttonNames = {
        "Играть",
        "Настройки",
        "Об игре",
        "Выход",
    };
};
