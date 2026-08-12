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

    auto& settings = AppSettings::instance();
    int sliderWidth = 300;

    std::vector<std::pair<int, int>> ranges = {
        {0, 180},
        {0, 3840},
        {0, 1000},
        {0, 500}
    };

    std::vector<int> defaultValues = {
        static_cast<int>(settings.fov() * 180 / 3.14159),
        settings.numberOfRays(),
        static_cast<int>(settings.renderDistance()),
        static_cast<int>(settings.playerSpeed())
    };

    for(size_t i = 0; i < sliderNames.size(); i++){
        QHBoxLayout *sliderLayout = new QHBoxLayout();

        QLabel *sliderName = new QLabel(sliderNames[i], this);
        sliderName->setFixedWidth(70);

        QLabel *sliderValue = new QLabel(QString::number(defaultValues[i]), this);
        sliderValue->setFixedWidth(40);
        sliderValue->setAlignment(Qt::AlignCenter);

        QSlider *slider = new QSlider(Qt::Horizontal, this);
        slider->setFixedWidth(sliderWidth);
        slider->setRange(ranges[i].first, ranges[i].second);
        slider->setValue(defaultValues[i]);
        sliders.push_back(slider);

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
            "QPushButton {background-color: #4CAF50; border: none; color: white; font-size: 14px; border-radius: 5px;}"
            "QPushButton:hover {background-color: #45a049;}"
            );
        buttonsLayout->addWidget(button);
        switch(i){
        case 0:
            connect(button, &QPushButton::clicked, this, &SettingsWindow::onSaveButtonClicked);
            break;
        case 1:
            connect(button, &QPushButton::clicked, this, &SettingsWindow::onDefaultButtonClicked);
            break;
        case 2:
            connect(button, &QPushButton::clicked, this, &SettingsWindow::onBackButtonClicked);
            break;
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

void SettingsWindow::onSaveButtonClicked()
{
    auto& settings = AppSettings::instance();

    double fovDeg = sliders[0]->value();
    settings.setFOV(fovDeg * M_PI / 180.0);
    settings.setNumberOfRays(sliders[1]->value());
    settings.setRenderDistance(sliders[2]->value());
    settings.setPlayerSpeed(sliders[3]->value());
    settings.sync();

}

void SettingsWindow::onDefaultButtonClicked()
{
    auto& settings = AppSettings::instance();

    settings.toDefaultSettings();

    sliders[0]->setValue(static_cast<int>(AppSettings::DEFAULT_FOV * 180 / M_PI));
    sliders[1]->setValue(static_cast<int>(AppSettings::DEFAULT_NUMBER_OF_RAYS));
    sliders[2]->setValue(static_cast<int>(AppSettings::DEFAULT_RENDER_DISTANCE));
    sliders[3]->setValue(static_cast<int>(AppSettings::DEFAULT_PLAYER_SPEED));

}

void SettingsWindow::onBackButtonClicked()
{
    emit backToMenu();
    close();
}
