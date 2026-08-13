#include <../headers/CreditsWindow.hpp>

CreditsWindow::CreditsWindow(QWidget *parent):QWidget(parent)
{
    initButtons();

    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}
void CreditsWindow::initButtons(){

    buttonsLayout->setAlignment(Qt::AlignCenter);

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);
        button->setStyleSheet(
            "QPushButton {background-color: #4CAF50; border: none; color: white; font-size: 14px; border-radius: 5px;}"
            "QPushButton:hover {background-color: #45a049;}"
            );
        buttonsLayout->addWidget(button);
        switch(i){
        case 0:
            connect(button, &QPushButton::clicked, this, &CreditsWindow::onBackButtonClicked);
            break;
        }
    }
}

void CreditsWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        onBackButtonClicked();
    }else{
        QWidget::keyPressEvent(event);
    }
}

void CreditsWindow::onBackButtonClicked(){
    emit backToMenu();
    close();
}