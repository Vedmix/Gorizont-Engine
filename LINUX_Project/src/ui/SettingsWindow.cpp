#include "../headers/SettingsWindow.hpp"

SettingsWindow::SettingsWindow(QWidget *parent): QWidget(parent)
{
    initSliders();
    initRadioButtons();
    initButtons();
    initNotification();

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

    std::vector<std::pair<int, int>> ranges = {
        {0, 180},
        {0, 3840},
        {0, 1000},
        {0, 500}
    };

    std::vector<int> defaultValues = {
        static_cast<int>(settings.fov() * 180 / M_PI),
        static_cast<int>(settings.numberOfRays()),
        static_cast<int>(settings.renderDistance()),
        static_cast<int>(settings.playerSpeed())
    };

    for(size_t i = 0; i < sliderNames.size(); i++){
        QHBoxLayout *sliderLayout = new QHBoxLayout();

        QLabel *sliderName = new QLabel(sliderNames[i], this);
        sliderName->setFixedWidth(80);

        QLabel *sliderValue = new QLabel(QString::number(defaultValues[i]), this);
        sliderValue->setFixedWidth(40);
        sliderValue->setAlignment(Qt::AlignCenter);

        sliderValue->setProperty("class", "valueLabel");

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


void SettingsWindow::initNotification()
{
    notificationWidget = new QWidget(this);
    notificationWidget->setObjectName("notificationWidget");
    notificationWidget->setFixedSize(180, 50);
    notificationWidget->hide();

    notificationLabel = new QLabel("Uved", notificationWidget);
    notificationLabel->setObjectName("notificationLabel");
    notificationLabel->setAlignment(Qt::AlignCenter);

    QVBoxLayout* layout = new QVBoxLayout(notificationWidget);
    layout->addWidget(notificationLabel);
    layout->setContentsMargins(0, 0, 0, 0);

    notificationAnimation = new QPropertyAnimation(notificationWidget, "geometry");
    notificationAnimation->setDuration(400);

    notificationTimer = new QTimer(this);
    notificationTimer->setSingleShot(true);
    connect(notificationTimer, &QTimer::timeout, this, &SettingsWindow::hideNotification);
}

void SettingsWindow::showNotification(const QString& text, bool success)
{

    notificationLabel->setText(text);

    notificationWidget->setProperty("success", success);
    notificationLabel->setProperty("success", success);
    notificationWidget->style()->polish(notificationWidget);
    notificationLabel->style()->polish(notificationLabel);

    int x = width() - notificationWidget->width() - 20;
    int y = 20;

    QRect startRect(x + notificationWidget->width(), y, notificationWidget->width(), notificationWidget->height());
    QRect endRect(x, y,notificationWidget->width(), notificationWidget->height());

    notificationWidget->setGeometry(startRect);
    notificationWidget->show();
    notificationWidget->raise();

    notificationAnimation->setStartValue(startRect);
    notificationAnimation->setEndValue(endRect);
    notificationAnimation->start();

    notificationTimer->start(2000);
}

void SettingsWindow::hideNotification()
{

    QRect currentRect = notificationWidget->geometry();
    QRect endRect(currentRect.x() + currentRect.width() + 20, currentRect.y(), currentRect.width(), currentRect.height());

    notificationAnimation->stop();
    notificationAnimation->setStartValue(currentRect);
    notificationAnimation->setEndValue(endRect);
    notificationAnimation->start();

    QTimer::singleShot(400, [this]() {
        if (notificationWidget) {
            notificationWidget->hide();
            notificationWidget->move(width() - notificationWidget->width() - 20, 20);
        }
    });
}

void SettingsWindow::initButtons(){

    buttonsLayout->setAlignment(Qt::AlignCenter);

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);
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

    showNotification("Сохранено!", true);

}

void SettingsWindow::onDefaultButtonClicked()
{
    auto& settings = AppSettings::instance();

    settings.toDefaultSettings();

    sliders[0]->setValue(static_cast<int>(AppSettings::DEFAULT_FOV * 180.0 / M_PI));
    sliders[1]->setValue(static_cast<int>(AppSettings::DEFAULT_NUMBER_OF_RAYS));
    sliders[2]->setValue(static_cast<int>(AppSettings::DEFAULT_RENDER_DISTANCE));
    sliders[3]->setValue(static_cast<int>(AppSettings::DEFAULT_PLAYER_SPEED));

    showNotification("Сброшено!", true);

}

void SettingsWindow::onBackButtonClicked()
{
    emit backToMenu();
    close();
}
