#include "../headers/MainWindow.h"

MainWindow::MainWindow(QWidget *parent, int choice)
    : QMainWindow(parent)
{
    if(choice == 0){
        initMenu();
    } else{
        openSfmlWindow();
    }
}

MainWindow::~MainWindow() {}

void MainWindow::initMenu(){
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

    connect(buttonGroup, &QButtonGroup::idClicked, this, &MainWindow::handleButton);//привязка списка конопок к обработчику

    mainLayout->addStretch();//добавлено растяжение компановки сверху

    hLayout->addStretch(); //добавлено растяжение компановки слева
    hLayout->addLayout(buttonLayout);//центр(кнопки)
    hLayout->addStretch();//добавлено растяжение компановки справа

    mainLayout->addLayout(hLayout);
    mainLayout->addStretch();//добавлено растяжение компановки снизк

    showFullScreen();
}

void MainWindow::handleButton(int id)
{
    switch(id) {
        case 0: //запуск игры
            openSfmlWindow();
            break;
        case 1: //настройки
            break;
        case 2: //авторы
            break;
        case 3://выход с закрытием окна
            this->close();
            break;
    };
}


void MainWindow::openSfmlWindow()
{
    this->hide();

    World world;

    world.run();
}
