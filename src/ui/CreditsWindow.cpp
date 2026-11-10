#include <../headers/CreditsWindow.hpp>

CreditsWindow::CreditsWindow(QWidget *parent):QWidget(parent)
{
    initButtons();
    initCredits();

    mainLayout->addStretch();
    mainLayout->addWidget(creditsLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
    showFullScreen();
}
void CreditsWindow::initButtons(){

    buttonsLayout->setAlignment(Qt::AlignCenter);

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);
        buttonsLayout->addWidget(button);
        switch(i){
        case 0:
            connect(button, &QPushButton::clicked, this, &CreditsWindow::onBackButtonClicked);
            break;
        }
    }
}

void CreditsWindow::initCredits()
{
    creditsLabel = new QLabel(this);
    creditsLabel->setText("Vedmix — Игрок на мышке\n   \nEnvelopeDev — Игрок на стрелочках\n  \n Greno4ka — Автор единственного верифицированного коммита\n   \nDerseTTing, tiliberkoo — Погибли при создании проекта");
    creditsLabel->setAlignment(Qt::AlignCenter);

    QFont font = creditsLabel->font();
    font.setPointSize(80);
    creditsLabel->setFont(font);
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
