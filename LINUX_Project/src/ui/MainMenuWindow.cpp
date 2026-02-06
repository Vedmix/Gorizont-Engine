#include "../headers/MainMenuWindow.hpp"

MainMenuWindow::MainMenuWindow(QWidget* parent, int choice)
    : QMainWindow(parent), gameWindow(nullptr)
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

MainMenuWindow::~MainMenuWindow() {}

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

    switch(id) {
    case 0: // Играть
        if (gameWindow == nullptr) {
            gameWindow = new GameWindow(nullptr);
            gameWindow->setGeometry(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
            gameWindow->setWindowTitle("Gorizont(QT MODE)");

            connect(gameWindow, &GameWindow::gameFinished, this, &MainMenuWindow::onGameFinished);
        }

        this->hide();

        gameWindow->showFullScreen();
        gameWindow->activateWindow();
        gameWindow->raise();
        gameWindow->startGame();

        playButton->setText("Продолжить");
        break;

    case 3: // Выход
        this->close();
        break;
    }
}

void MainMenuWindow::onGameFinished()
{

    if (gameWindow) {
        gameWindow->hide();
    }

    this->show();
    this->activateWindow();
    this->raise();

}
