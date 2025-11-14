#include "main_window.h"
#include <QColorDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    animationRunning(false) {

    qDebug() << "MainWindow constructor";

    // Создание центрального виджета
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Создание layout
    mainLayout = new QVBoxLayout(centralWidget);
    controlLayout = new QHBoxLayout();

    // Создание элементов управления
    titleLabel = new QLabel("SFML + Qt5 Integration Test", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px; color: white; background-color: #2b2b2b; padding: 10px;");

    sizeLabel = new QLabel("Circle Size: 50", this);
    sizeLabel->setStyleSheet("color: white; background-color: #3b3b3b; padding: 5px;");

    colorButton = new QPushButton("Change Color", this);
    animationButton = new QPushButton("Start Animation", this);

    // Стилизация кнопок
    colorButton->setStyleSheet("QPushButton { background-color: #4CAF50; color: white; border: none; padding: 8px; }"
                               "QPushButton:hover { background-color: #45a049; }");
    animationButton->setStyleSheet("QPushButton { background-color: #2196F3; color: white; border: none; padding: 8px; }"
                                   "QPushButton:hover { background-color: #0b7dda; }");

    sizeSlider = new QSlider(Qt::Horizontal, this);
    sizeSlider->setRange(10, 200);
    sizeSlider->setValue(50);

    // SFML виджет
    sfmlWidget = new SFMLWidget(this);
    sfmlWidget->setMinimumSize(400, 300);
    sfmlWidget->setStyleSheet("background-color: #1a1a1a;");

    // Таймер для анимации
    animationTimer = new QTimer(this);

    // Настройка layout
    controlLayout->addWidget(sizeLabel);
    controlLayout->addWidget(sizeSlider);
    controlLayout->addWidget(colorButton);
    controlLayout->addWidget(animationButton);

    mainLayout->addWidget(titleLabel);
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(sfmlWidget, 1);

    // Устанавливаем темный фон
    centralWidget->setStyleSheet("background-color: #2b2b2b;");

    // Подключение сигналов
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::onColorChanged);
    connect(sizeSlider, &QSlider::valueChanged, this, &MainWindow::onSizeChanged);
    connect(animationButton, &QPushButton::clicked, this, &MainWindow::onToggleAnimation);
    connect(animationTimer, &QTimer::timeout, sfmlWidget, &SFMLWidget::updateAnimation);

    // Инициализируем SFML сразу
    QTimer::singleShot(100, sfmlWidget, [this]() {
        qDebug() << "Manually initializing SFML...";
        sfmlWidget->initializeSFML();
    });

    qDebug() << "MainWindow setup complete";
}

void MainWindow::onColorChanged() {
    qDebug() << "Color change requested";
    QColor color = QColorDialog::getColor(Qt::green, this, "Select Circle Color");
    if (color.isValid()) {
        sfmlWidget->setCircleColor(color.red(), color.green(), color.blue());
    }
}

void MainWindow::onSizeChanged(int value) {
    sizeLabel->setText(QString("Circle Size: %1").arg(value));
    sfmlWidget->setCircleRadius(value);
}

void MainWindow::onToggleAnimation() {
    animationRunning = !animationRunning;

    if (animationRunning) {
        animationTimer->start(16); // ~60 FPS
        animationButton->setText("Stop Animation");
        qDebug() << "Animation started";
    } else {
        animationTimer->stop();
        animationButton->setText("Start Animation");
        qDebug() << "Animation stopped";
    }
}