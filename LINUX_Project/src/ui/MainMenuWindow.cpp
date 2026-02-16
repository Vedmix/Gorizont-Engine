#include "../headers/MainMenuWindow.hpp"
#include "../headers/GameWindow.hpp"
#include "../headers/SettingsWindow.hpp"
#include <QDebug>
#include <cmath>

MainMenuWindow::MainMenuWindow(QWidget* parent, int choice)
    : QMainWindow(parent), gameWindow(nullptr), settingsWindow(nullptr)
{
    if(choice == 0){
        initMenu();
    } else{
        if(gameWindow == nullptr){
            gameWindow = new GameWindow(this);
            connect(gameWindow, &GameWindow::gameFinished, this, &MainMenuWindow::onGameFinished);
        }
        gameWindow->startGame();
    }
}

MainMenuWindow::~MainMenuWindow()
{
    // Qt автоматически удалит дочерние виджеты
}

void MainMenuWindow::initMenu(){
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(15);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *hLayout = new QHBoxLayout();

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button;
        if(i == 0){
            button = new QPushButton(buttonNames[i], this);
            playButton = button;
        }else{
            button = new QPushButton(buttonNames[i], this);
        }
        button->setFixedSize(200, 50);

        buttonGroup->addButton(button, static_cast<int>(i));
        buttonLayout->addWidget(button);

        button->setStyleSheet(
            "QPushButton {"
            "    background-color: #4CAF50;"
            "    border: none;"
            "    color: white;"
            "    font-size: 14px;"
            "    border-radius: 5px;"
            "}"
            "QPushButton:hover {"
            "    background-color: #45a049;"
            "}"
            );
    }

    connect(buttonGroup, &QButtonGroup::idClicked, this, &MainMenuWindow::handleButton);

    mainLayout->addStretch();
    hLayout->addStretch();
    hLayout->addLayout(buttonLayout);
    hLayout->addStretch();
    mainLayout->addLayout(hLayout);
    mainLayout->addStretch();

    showFullScreen();
}

void MainMenuWindow::handleButton(int id)
{
    qDebug() << "Button clicked:" << id; // Для отладки

    switch(id) {
    case 0: // Играть
    {
        qDebug() << "Opening Game Window";
        if (gameWindow == nullptr) {
            gameWindow = new GameWindow(); // Без parent, чтобы было отдельное окно
            gameWindow->setWindowTitle("Gorizont(QT MODE)");
            gameWindow->setAttribute(Qt::WA_DeleteOnClose, false);
            connect(gameWindow, &GameWindow::gameFinished, this, &MainMenuWindow::onGameFinished);
        }

        this->hide();
        gameWindow->showFullScreen();
        gameWindow->activateWindow();
        gameWindow->raise();
        gameWindow->startGame();

        playButton->setText("Продолжить");
    }
    break;

    case 1: // Настройки
    {
        qDebug() << "Opening Settings Window";
        // Создаем новое окно настроек каждый раз
        if (settingsWindow) {
            settingsWindow->deleteLater();
            settingsWindow = nullptr;
        }

        settingsWindow = new SettingsWindow(); // Без parent
        settingsWindow->setWindowTitle("Настройки");
        settingsWindow->setAttribute(Qt::WA_DeleteOnClose, false);

        connect(settingsWindow, &SettingsWindow::backToMenu, this, &MainMenuWindow::onSettingsClosed);

        this->hide();
        settingsWindow->showFullScreen();
        settingsWindow->activateWindow();
        settingsWindow->raise();
    }
    break;

    case 2: // Авторы
    {
        QMessageBox::information(this, "Авторы", "Разработчик: Ваше имя\nГод: 2024");
    }
    break;

    case 3: // Выход
        qDebug() << "Exiting application";
        QApplication::quit();
        break;
    }
}

void MainMenuWindow::onGameFinished()
{
    qDebug() << "Game finished, returning to menu";
    if (gameWindow) {
        gameWindow->hide();
        // Не удаляем, чтобы сохранить состояние игры
    }

    this->showFullScreen();
    this->activateWindow();
    this->raise();
}

void MainMenuWindow::onSettingsClosed()
{
    qDebug() << "Settings closed, returning to menu";
    if (settingsWindow) {
        settingsWindow->hide();
        settingsWindow->deleteLater(); // Удалим при возврате
        settingsWindow = nullptr;
    }

    this->showFullScreen();
    this->activateWindow();
    this->raise();
}
