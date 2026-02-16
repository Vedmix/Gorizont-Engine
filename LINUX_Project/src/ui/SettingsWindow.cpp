#include "../headers/SettingsWindow.hpp"
#include <QLayout>
#include <QSlider>
#include <QLabel>
#include <QMessageBox>
#include <QApplication>
#include <QDebug>
#include <QCloseEvent>

SettingsWindow::SettingsWindow(QWidget *parent): QWidget(parent)
{
    qDebug() << "SettingsWindow created";

    // Важно! Устанавливаем размер и политику
    setWindowFlags(Qt::Window); // Делаем полноценным окном
    setAttribute(Qt::WA_DeleteOnClose, false);

    // Инициализируем UI
    initSliders();
    initRadioButtons();
    initButtons();

    mainLayout->addLayout(slidersLayout);
    mainLayout->addLayout(radioButtonsLayout);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);

    qDebug() << "SettingsWindow initialized";
}

QString SettingsWindow::getGraphicsText(int value) const
{
    if (value >= 1 && value <= 16) {
        return "Low";
    } else if (value >= 17 && value <= 32) {
        return "Medium";
    } else if (value >= 33 && value <= 48) {
        return "High";
    } else if (value >= 49 && value <= 64) {
        return "ULTRA";
    } else {
        return QString::number(value);
    }
}

void SettingsWindow::initSliders(){
    slidersLayout->setAlignment(Qt::AlignCenter);
    slidersLayout->addStretch();

    // FOV слайдер
    {
        QHBoxLayout *sliderLayout = new QHBoxLayout();
        QLabel *sliderName = new QLabel("FOV", this);
        sliderName->setFixedWidth(120);
        QLabel *sliderValue = new QLabel("90", this);
        sliderValue->setFixedWidth(50);
        fovSlider = new QSlider(Qt::Horizontal, this);
        fovSlider->setMaximumWidth(400);
        fovSlider->setRange(30, 150);
        fovSlider->setValue(90);

        sliderLayout->addWidget(sliderName);
        sliderLayout->addWidget(sliderValue);
        sliderLayout->addWidget(fovSlider);
        slidersLayout->addLayout(sliderLayout);

        connect(fovSlider, &QSlider::valueChanged, this, [sliderValue](double value){
            sliderValue->setText(QString::number(value));
        });
    }

    // Graphics слайдер
    {
        QHBoxLayout *sliderLayout = new QHBoxLayout();
        QLabel *sliderName = new QLabel("Graphics (rays)", this);
        sliderName->setFixedWidth(120);
        QLabel *sliderValue = new QLabel("Medium", this);
        sliderValue->setFixedWidth(50);
        graphicsSlider = new QSlider(Qt::Horizontal, this);
        graphicsSlider->setMaximumWidth(400);
        graphicsSlider->setRange(1, 64);
        graphicsSlider->setValue(32); // Устанавливаем начальное значение

        sliderLayout->addWidget(sliderName);
        sliderLayout->addWidget(sliderValue);
        sliderLayout->addWidget(graphicsSlider);
        slidersLayout->addLayout(sliderLayout);

        connect(graphicsSlider, &QSlider::valueChanged, this,
                [this, sliderValue](int value) {
                    sliderValue->setText(this->getGraphicsText(value));
                });
    }

    // Distance слайдер сделать числовым блоком
    {
        QHBoxLayout *sliderLayout = new QHBoxLayout();
        QLabel *sliderName = new QLabel("Distance", this);
        sliderName->setFixedWidth(120);
        QLabel *sliderValue = new QLabel("1000", this);
        sliderValue->setFixedWidth(50);
        distanceSlider = new QSlider(Qt::Horizontal, this);
        distanceSlider->setMaximumWidth(400);
        distanceSlider->setRange(100, 5112);
        distanceSlider->setValue(1000);

        sliderLayout->addWidget(sliderName);
        sliderLayout->addWidget(sliderValue);
        sliderLayout->addWidget(distanceSlider);
        slidersLayout->addLayout(sliderLayout);

        connect(distanceSlider, &QSlider::valueChanged, this, [sliderValue](int value){
            sliderValue->setText(QString::number(value));
        });
    }

    // Speed слайдер сделать числовым блоком
    {
        QHBoxLayout *sliderLayout = new QHBoxLayout();
        QLabel *sliderName = new QLabel("Speed", this);
        sliderName->setFixedWidth(120);
        QLabel *sliderValue = new QLabel("150", this);
        sliderValue->setFixedWidth(50);
        speedSlider = new QSlider(Qt::Horizontal, this);
        speedSlider->setMaximumWidth(400);
        speedSlider->setRange(50, 500);
        speedSlider->setValue(150);

        sliderLayout->addWidget(sliderName);
        sliderLayout->addWidget(sliderValue);
        sliderLayout->addWidget(speedSlider);
        slidersLayout->addLayout(sliderLayout);

        connect(speedSlider, &QSlider::valueChanged, this, [sliderValue](int value){
            sliderValue->setText(QString::number(value));
        });
    }
}

void SettingsWindow::initRadioButtons(){
    drugsRadioButton = new QRadioButton("DRUGS MOD", this);
    drugsRadioButton->setFixedSize(200, 50);
    radioButtonsLayout->addWidget(drugsRadioButton);
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

        if (i == 0){ // Сохранить
            connect(button, &QPushButton::clicked, this, [this](){
                QMessageBox::information(this, "Сохранение", "Настройки сохранены");
            });
        }
        else if (i == 1){ // По умолчанию
            connect(button, &QPushButton::clicked, this, [this](){
                fovSlider->setValue(90);
                graphicsSlider->setValue(960);
                distanceSlider->setValue(1000);
                speedSlider->setValue(150);
                drugsRadioButton->setChecked(false);
            });
        }
        else if (i == 2){ // Выход
            connect(button, &QPushButton::clicked, this, &SettingsWindow::onBackButtonClicked);
        }
    }
}

void SettingsWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        onBackButtonClicked();
    } else {
        QWidget::keyPressEvent(event);
    }
}

void SettingsWindow::onBackButtonClicked()
{
    qDebug() << "SettingsWindow: back button clicked";
    hide();
    emit backToMenu();
}

void SettingsWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "SettingsWindow: close event";
    onBackButtonClicked();
    event->accept();
}
