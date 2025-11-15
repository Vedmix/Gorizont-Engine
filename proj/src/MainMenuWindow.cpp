#include "../headers/MainMenuWindow.h"

MainMenuWindow::MainMenuWindow(QWidget *parent,int choice)
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
    QWidget *centralWidget = new QWidget(this);//контейнер для отцентрованных элементов
    setCentralWidget(centralWidget);

    QButtonGroup *buttonGroup = new QButtonGroup(this);//список кнопок

    QVBoxLayout *buttonLayout = new QVBoxLayout();//компановка кнопок
    buttonLayout->setSpacing(15);//промежуток между кнопками

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);//главная компановка


    QHBoxLayout *hLayout = new QHBoxLayout();//компановка по горизонтали

    for(int i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);

        buttonGroup->addButton(button, i);
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

    connect(buttonGroup, &QButtonGroup::idClicked, this, &MainMenuWindow::handleButton);//привязка списка конопок к обработчику

    mainLayout->addStretch();//добавлено растяжение компановки сверху

    hLayout->addStretch(); //добавлено растяжение компановки слева
    hLayout->addLayout(buttonLayout);//центр(кнопки)
    hLayout->addStretch();//добавлено растяжение компановки справа

    mainLayout->addLayout(hLayout);
    mainLayout->addStretch();//добавлено растяжение компановки снизк

    showFullScreen();
}

void MainMenuWindow::handleButton(int id)
{
    switch(id) {
    case 0: // Играть
        if (!gameWindow) {
            gameWindow = new GameWindow();
            gameWindow->setWindowTitle("GAME WINDOW - TEST");
            gameWindow->resize(800, 600); // Явно устанавливаем размер
            connect(gameWindow, &GameWindow::gameFinished, this, &MainMenuWindow::onGameFinished);
        }

        this->hide();

        // Показываем в центре экрана
        gameWindow->move(100, 100); // Явно устанавливаем позицию
        gameWindow->show();
        gameWindow->activateWindow();
        gameWindow->raise();

        // Принудительные обновления
        gameWindow->update();
        gameWindow->repaint();

        gameWindow->startGame();
        break;
    case 3: // Выход
        this->close();
        break;
    }
}


void MainMenuWindow::onGameFinished()
{
    gameWindow->hide();
    this->show();
}
