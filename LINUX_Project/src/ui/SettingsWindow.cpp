#include "../headers/SettingsWindow.hpp"

SettingsWindow::SettingsWindow(QWidget *parent): QWidget(parent)
{
    initSliders();
    initRadioButtons();
    initButtons();

    mainLayout->addLayout(slidersLayout);
    mainLayout->addLayout(radioButtonsLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

void SettingsWindow::initSliders(){

    slidersLayout->setAlignment(Qt::AlignCenter);
    slidersLayout->addStretch();

    for(size_t i = 0; i < sliderNames.size(); i++){
        QHBoxLayout *sliderLayout = new QHBoxLayout();

        QLabel *sliderName = new QLabel(sliderNames[i], this);
        sliderName->setFixedWidth(70);

        QLabel *sliderValue= new QLabel(QString::number(i), this);
        sliderValue->setFixedWidth(20);

        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setMaximumWidth(300);

        sliderLayout->addWidget(sliderName);
        sliderLayout->addWidget(sliderValue);
        sliderLayout->addWidget(slider);

        slidersLayout->addLayout(sliderLayout);
    }
}

void SettingsWindow::initRadioButtons(){

    for(size_t i = 0; i < radioButtonNames.size(); i++){
        QRadioButton *radioButton = new QRadioButton(radioButtonNames[i], this);
        radioButton->setFixedSize(200, 50);

        radioButtonsLayout->addWidget(radioButton);
    }
}

void SettingsWindow::initButtons(){

    buttonsLayout->setAlignment(Qt::AlignCenter);

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);
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
        buttonsLayout->addWidget(button);

        if (i == 2){
            connect(button, &QPushButton::clicked, this, &SettingsWindow::onBackButtonClicked);
        }
    }
}

void SettingsWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        onBackButtonClicked();
    }else{
        QWidget::keyPressEvent(event);
    }
}

void SettingsWindow::onBackButtonClicked()
{
    emit backToMenu();
    close();
}
