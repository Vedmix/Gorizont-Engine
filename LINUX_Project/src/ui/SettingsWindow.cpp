#include "../headers/SettingsWindow.hpp"

SettingsWindow::SettingsWindow(QWidget *parent): QWidget(parent)
{
    initUI();

}

void SettingsWindow::initUI(){
    mainLayout = new QVBoxLayout(this);
    settingsLayout = new QHBoxLayout();

    initGameSettings();
    initInterfaceSettings();

    settingsLayout->addWidget(gameSettingsGroup);
    settingsLayout->addWidget(interfaceSettingsGroup);

    initButtons();
    initNotification();

    mainLayout->addLayout(settingsLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);

    AppSettings::applyTheme(AppSettings::instance().theme());


}

void SettingsWindow::initGameSettings(){
    gameSettingsGroup = new QGroupBox("Игра", this);
    gameSettingsGroup->setFixedWidth(960);
    gameSettingsGroup->setFixedHeight(300);
    gameSettingsLayout = new QVBoxLayout(gameSettingsGroup);

    sliderLayout = new QVBoxLayout();
    radioLayout = new QHBoxLayout();


    initSliders();
    initRadioButton();

    gameSettingsLayout->addLayout(sliderLayout);
    gameSettingsLayout->addLayout(radioLayout);
    gameSettingsLayout->addStretch();
}

void SettingsWindow::initInterfaceSettings() {
    interfaceSettingsGroup = new QGroupBox("Интерфейс", this);
    interfaceSettingsGroup->setFixedWidth(920);
    interfaceSettingsGroup->setFixedHeight(300);
    interfaceSettingsLayout = new QVBoxLayout(interfaceSettingsGroup);

    themeLayout = new QHBoxLayout();
    mapLayout = new QHBoxLayout();

    initThemeSelector();
    initMapSelector();

    interfaceSettingsLayout->addLayout(themeLayout);
    interfaceSettingsLayout->addLayout(mapLayout);
    interfaceSettingsLayout->addStretch();
}

void SettingsWindow::initSliders(){
    auto& settings = AppSettings::instance();

    std::vector<int> slidersDefaultValues = {
        static_cast<int>(settings.fov() * 180 / M_PI),
        settings.numberOfRays(),
        static_cast<int>(settings.renderDistance()),
        static_cast<int>(settings.playerSpeed())
    };

    for (size_t i = 0; i < sliderNames.size(); i++) {
        QHBoxLayout* l_sliderLayout = new QHBoxLayout();
        QLabel* nameLabel = new QLabel(sliderNames[i], this);
        nameLabel->setFixedWidth(80);

        QLabel* valueLabel = new QLabel(QString::number(slidersDefaultValues[i]), this);
        valueLabel->setFixedWidth(40);
        valueLabel->setAlignment(Qt::AlignCenter);
        valueLabel->setProperty("class", "valueLabel");
        sliderValueLabels.push_back(valueLabel);

        QSlider* slider = new QSlider(Qt::Horizontal, this);
        slider->setFixedWidth(sliderWidth);
        slider->setRange(slidersRanges[i].first, slidersRanges[i].second);
        slider->setValue(slidersDefaultValues[i]);
        gameSliders.push_back(slider);

        connect(slider, &QSlider::valueChanged, [valueLabel](int value) {valueLabel->setText(QString::number(value));});

        l_sliderLayout->addWidget(nameLabel);
        l_sliderLayout->addWidget(valueLabel);
        l_sliderLayout->addWidget(slider);
        l_sliderLayout->addStretch();
        sliderLayout->addLayout(l_sliderLayout);
    }
}

void SettingsWindow::initMapSelector(){
    auto& settings = AppSettings::instance();
    static QPushButton* selectMapButton = nullptr;
    QLabel* mapLabel = new QLabel("Карта:", this);
    mapLabel->setFixedWidth(120);

    mapComboBox = new QComboBox(this);
    mapComboBox->setFixedWidth(150);
    mapComboBox->addItem("map1.xml");
    mapComboBox->addItem("map2.xml");

    QString currentMap = settings.mapPath();
    int index = mapComboBox->findText(QFileInfo(currentMap).fileName());
    if (index >= 0) {
        mapComboBox->setCurrentIndex(index);
    }

    selectMapButton = new QPushButton("Обзор...", this);
    selectMapButton->setFixedSize(200, 50);
    connect(selectMapButton, &QPushButton::clicked, this, &SettingsWindow::onSelectMapClicked);

    mapLayout->addWidget(mapLabel);
    mapLayout->addWidget(mapComboBox);
    mapLayout->addWidget(selectMapButton);
    mapLayout->addStretch();
}

void SettingsWindow::initRadioButton(){
    static QRadioButton* drugsRadioButton = nullptr;
    drugsRadioButton = new QRadioButton("DRUGS MOD", this);
    drugsRadioButton->setFixedSize(200, 50);
    radioLayout->addWidget(drugsRadioButton);
    radioLayout->addStretch();
}

void SettingsWindow::initThemeSelector(){
    auto& settings = AppSettings::instance();
    QLabel* themeLabel = new QLabel("Тема:", this);
    themeLabel->setFixedWidth(80);

    themeComboBox = new QComboBox(this);
    themeComboBox->setFixedWidth(200);

    QStringList themes = settings.availableThemes();
    for (const QString& theme : themes) {
        themeComboBox->addItem(theme);
    }

    QString currentTheme = settings.theme();
    int themeIndex = themeComboBox->findText(currentTheme);
    if (themeIndex >= 0) {
        themeComboBox->setCurrentIndex(themeIndex);
    }

    themeLayout->addWidget(themeLabel);
    themeLayout->addWidget(themeComboBox);
    themeLayout->addStretch();
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
    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setAlignment(Qt::AlignCenter);
    buttonsLayout->setSpacing(20);


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

    double fovDeg = gameSliders[0]->value();
    settings.setFOV(fovDeg * M_PI / 180.0);
    settings.setNumberOfRays(gameSliders[1]->value());
    settings.setRenderDistance(gameSliders[2]->value());
    settings.setPlayerSpeed(gameSliders[3]->value());

    QString mapName = mapComboBox->currentText();


    QString themeName = themeComboBox->currentText();
    settings.setTheme(themeName);

    AppSettings::applyTheme(themeName);
    AppSettings::applyMap("maps/" + mapName);

    settings.sync();

    showNotification("Сохранено!", true);

}

void SettingsWindow::onDefaultButtonClicked()

{
    auto& settings = AppSettings::instance();

    settings.toDefaultSettings();

    gameSliders[0]->setValue(static_cast<int>(AppSettings::DEFAULT_FOV * 180.0 / M_PI));
    gameSliders[1]->setValue(static_cast<int>(AppSettings::DEFAULT_NUMBER_OF_RAYS));
    gameSliders[2]->setValue(static_cast<int>(AppSettings::DEFAULT_RENDER_DISTANCE));
    gameSliders[3]->setValue(static_cast<int>(AppSettings::DEFAULT_PLAYER_SPEED));


    showNotification("Сброшено!", true);

}

void SettingsWindow::onSelectMapClicked() {
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Выберите карту",
        "maps/",
        "XML файлы (*.xml)"
        );

    if (!filePath.isEmpty()) {
        QString fileName = QFileInfo(filePath).fileName();
        int index = mapComboBox->findText(fileName);
        if (index >= 0) {
            mapComboBox->setCurrentIndex(index);
        } else {
            mapComboBox->addItem(fileName);
            mapComboBox->setCurrentIndex(mapComboBox->count() - 1);
        }
        showNotification("Карта выбрана", true);
    }
}


void SettingsWindow::onBackButtonClicked()
{
    emit backToMenu();
    close();
}
