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

    int sliderWidth = 300;

    std::vector<std::pair<int, int>> ranges = {
        {0, 180},
        {0, 3840},
        {0, 1000},
        {0, 500}
    };

    for(size_t i = 0; i < sliderNames.size(); i++){
        QHBoxLayout *sliderLayout = new QHBoxLayout();

        QLabel *sliderName = new QLabel(sliderNames[i], this);
        sliderName->setFixedWidth(70);

        QLabel *sliderValue = new QLabel("0", this);
        sliderValue->setFixedWidth(40);
        sliderValue->setAlignment(Qt::AlignCenter);

        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setFixedWidth(sliderWidth);
        slider->setRange(ranges[i].first, ranges[i].second);
        slider->setValue(0);

        connect(slider, &QSlider::valueChanged, [sliderValue](int value) {
            sliderValue->setText(QString::number(value));
        });

        sliderLayout->addWidget(sliderName);
        sliderLayout->addWidget(sliderValue);
        sliderLayout->addWidget(slider);

        slidersLayout->addLayout(sliderLayout);
    }
}

void SettingsWindow::initRadioButtons(){
    radioButtonsLayout->setAlignment(Qt::AlignCenter);
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
