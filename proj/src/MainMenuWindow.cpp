#include "../headers/MainMenuWindow.hpp"
#include <iostream>

MainMenuWindow::MainMenuWindow(QWidget* parent, int choice)
    : QMainWindow(parent), gameWindow(nullptr)
{
    std::cout << "MainMenuWindow constructor started" << std::endl;

    if(choice == 0){
        initMenu();
    } else{
        if(gameWindow == nullptr){
            gameWindow = new GameWindow(this);
            connect(gameWindow, &GameWindow::gameFinished, this, &MainMenuWindow::onGameFinished);
        }
        gameWindow->startGame();
    }

    std::cout << "MainMenuWindow constructor finished" << std::endl;
}

MainMenuWindow::~MainMenuWindow() {}

void MainMenuWindow::initMenu(){
    std::cout << "Initializing menu..." << std::endl;

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(15);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *hLayout = new QHBoxLayout();

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
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

    std::cout << "Menu initialized, showing fullscreen..." << std::endl;
    showFullScreen();
    std::cout << "Main menu should be visible now" << std::endl;
}
void MainMenuWindow::handleButton(int id)
{
    std::cout << "Button clicked: " << id << std::endl;

    switch(id) {
    case 0: // Играть
        std::cout << "Creating game window..." << std::endl;

        if (!gameWindow) {
            gameWindow = new GameWindow(nullptr);

            // Устанавливаем позицию и размер
            gameWindow->setGeometry(100, 100, SCREEN_WIDTH, SCREEN_HEIGHT);
            gameWindow->setWindowTitle("3D Raycasting Engine");

            connect(gameWindow, &GameWindow::gameFinished, this, &MainMenuWindow::onGameFinished);
            std::cout << "GameWindow created" << std::endl;
        }

        std::cout << "Hiding main menu..." << std::endl;
        this->hide();

        std::cout << "Showing game window..." << std::endl;
        gameWindow->showNormal();
        gameWindow->activateWindow();
        gameWindow->raise();

        std::cout << "Starting game..." << std::endl;
        gameWindow->startGame();

        std::cout << "Game started successfully" << std::endl;
        break;

    case 3: // Выход
        std::cout << "Exiting..." << std::endl;
        this->close();
        break;
    }
}

void MainMenuWindow::onGameFinished()
{
    std::cout << "Game finished, returning to main menu..." << std::endl;

    if (gameWindow) {
        gameWindow->hide();
    }

    this->show();
    this->activateWindow();
    this->raise();

    std::cout << "Main menu should be visible now" << std::endl;
}
