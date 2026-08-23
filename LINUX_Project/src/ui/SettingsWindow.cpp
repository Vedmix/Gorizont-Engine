#include "../headers/SettingsWindow.hpp"

SettingsWindow::SettingsWindow(QWidget *parent): QWidget(parent)
{
    initUI();

}

void SettingsWindow::initUI(){
    m_mainLayout = new QVBoxLayout(this);
    m_contentLayout = new QHBoxLayout();

    initGameSettings();
    initInterfaceSettings();

    m_contentLayout->addWidget(m_gameGroup);
    m_contentLayout->addWidget(m_interfaceGroup);

    initButtons();
    initNotification();

    m_mainLayout->addLayout(m_contentLayout);
    m_mainLayout->addLayout(m_buttonsLayout);

    setLayout(m_mainLayout);

    AppSettings::applyTheme(AppSettings::instance().theme());


}

void SettingsWindow::initGameSettings(){
    m_gameGroup = new QGroupBox("Игра", this);
    m_gameGroup->setFixedWidth(960);
    m_gameGroup->setFixedHeight(300);
    m_gameLayout = new QVBoxLayout(m_gameGroup);


    initSliders();
    initRadioButton();

    m_gameLayout->addLayout(sliderLayout);
    m_gameLayout->addLayout(radioLayout);
    m_gameLayout->addStretch();
}

void SettingsWindow::initInterfaceSettings() {
    m_interfaceGroup = new QGroupBox("Интерфейс", this);
    m_interfaceGroup->setFixedWidth(920);
    m_interfaceGroup->setFixedHeight(300);
    m_interfaceLayout = new QVBoxLayout(m_interfaceGroup);

    initThemeSelector();
    initMapSelector();

    m_interfaceLayout->addLayout(themeLayout);
    m_interfaceLayout->addLayout(mapLayout);
    m_interfaceLayout->addStretch();
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
        m_sliderValueLabels.push_back(valueLabel);

        QSlider* slider = new QSlider(Qt::Horizontal, this);
        slider->setFixedWidth(sliderWidth);
        slider->setRange(slidersRanges[i].first, slidersRanges[i].second);
        slider->setValue(slidersDefaultValues[i]);
        m_gameSliders.push_back(slider);

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
    QLabel* mapLabel = new QLabel("Карта:", this);
    mapLabel->setFixedWidth(120);

    m_mapComboBox = new QComboBox(this);
    m_mapComboBox->setFixedWidth(150);
    m_mapComboBox->addItem("map1.xml");
    m_mapComboBox->addItem("map2.xml");

    QString currentMap = settings.mapPath();
    int index = m_mapComboBox->findText(QFileInfo(currentMap).fileName());
    if (index >= 0) {
        m_mapComboBox->setCurrentIndex(index);
    }

    m_selectMapButton = new QPushButton("Обзор...", this);
    m_selectMapButton->setFixedSize(200, 50);
    connect(m_selectMapButton, &QPushButton::clicked, this, &SettingsWindow::onSelectMapClicked);

    mapLayout->addWidget(mapLabel);
    mapLayout->addWidget(m_mapComboBox);
    mapLayout->addWidget(m_selectMapButton);
    mapLayout->addStretch();
}

void SettingsWindow::initRadioButton(){
    m_drugsRadioButton = new QRadioButton("DRUGS MOD", this);
    m_drugsRadioButton->setFixedSize(200, 50);
    radioLayout->addWidget(m_drugsRadioButton);
    radioLayout->addStretch();
}

void SettingsWindow::initThemeSelector(){
    auto& settings = AppSettings::instance();

    QLabel* themeLabel = new QLabel("Тема:", this);
    themeLabel->setFixedWidth(80);

    m_themeComboBox = new QComboBox(this);
    m_themeComboBox->setFixedWidth(200);

    QStringList themes = settings.availableThemes();
    for (const QString& theme : themes) {
        m_themeComboBox->addItem(theme);
    }

    QString currentTheme = settings.theme();
    int themeIndex = m_themeComboBox->findText(currentTheme);
    if (themeIndex >= 0) {
        m_themeComboBox->setCurrentIndex(themeIndex);
    }

    themeLayout->addWidget(themeLabel);
    themeLayout->addWidget(m_themeComboBox);
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
    m_buttonsLayout = new QHBoxLayout();
    m_buttonsLayout->setAlignment(Qt::AlignCenter);
    m_buttonsLayout->setSpacing(20);


    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);
        m_buttonsLayout->addWidget(button);
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

    double fovDeg = m_gameSliders[0]->value();
    settings.setFOV(fovDeg * M_PI / 180.0);
    settings.setNumberOfRays(m_gameSliders[1]->value());
    settings.setRenderDistance(m_gameSliders[2]->value());
    settings.setPlayerSpeed(m_gameSliders[3]->value());

    QString mapName = m_mapComboBox->currentText();
    settings.setMapPath("maps/" + mapName);

    QString themeName = m_themeComboBox->currentText();
    settings.setTheme(themeName);
    AppSettings::applyTheme(themeName);

    settings.sync();

    showNotification("Сохранено!", true);

}

void SettingsWindow::onDefaultButtonClicked()
{
    auto& settings = AppSettings::instance();

    settings.toDefaultSettings();

    m_gameSliders[0]->setValue(static_cast<int>(AppSettings::DEFAULT_FOV * 180.0 / M_PI));
    m_gameSliders[1]->setValue(static_cast<int>(AppSettings::DEFAULT_NUMBER_OF_RAYS));
    m_gameSliders[2]->setValue(static_cast<int>(AppSettings::DEFAULT_RENDER_DISTANCE));
    m_gameSliders[3]->setValue(static_cast<int>(AppSettings::DEFAULT_PLAYER_SPEED));


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
        int index = m_mapComboBox->findText(fileName);
        if (index >= 0) {
            m_mapComboBox->setCurrentIndex(index);
        } else {
            m_mapComboBox->addItem(fileName);
            m_mapComboBox->setCurrentIndex(m_mapComboBox->count() - 1);
        }
        showNotification("Карта выбрана", true);
    }
}


void SettingsWindow::onBackButtonClicked()
{
    emit backToMenu();
    close();
}
