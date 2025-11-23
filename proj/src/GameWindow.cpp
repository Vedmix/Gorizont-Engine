#include "../headers/GameWindow.hpp"
#include <iostream>
#include <QApplication>
#include <cmath>

GameWindow::GameWindow(QWidget* parent)
    : QWidget(parent),
    m_worldAdapter(),
    m_timer(new QTimer(this)),
    m_frameCount(0)
{
    std::cout << "GameWindow constructor started" << std::endl;

    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);

    connect(m_timer, &QTimer::timeout, this, &GameWindow::onUpdate);
    setStyleSheet("background-color: black;");

    std::cout << "GameWindow constructor finished" << std::endl;
}

// ДОБАВЛЯЕМ ДЕСТРУКТОР
GameWindow::~GameWindow()
{
    std::cout << "GameWindow destructor" << std::endl;
    if (m_timer) {
        m_timer->stop();
        delete m_timer;
    }
}

void GameWindow::startGame() {
    std::cout << "GameWindow::startGame() called" << std::endl;
    m_timer->start(16); // 60 FPS
    setFocus();
}

void GameWindow::stopGame() {
    std::cout << "GameWindow::stopGame() called" << std::endl;
    m_timer->stop();
}

void GameWindow::onUpdate() {
    m_frameCount++;

    // Обновление WorldAdapter
    double deltaTime = 16.0 / 1000.0;
    m_worldAdapter.update(deltaTime);

    update();

    if (m_frameCount % 60 == 0) {
        std::cout << "Frame " << m_frameCount << " - WorldAdapter updated" << std::endl;
    }
}

void GameWindow::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)

    QPainter painter(this);

    // Рендеринг через WorldAdapter
    sf::RenderTexture texture;
    if (texture.create(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        try {
            m_worldAdapter.renderToTexture(texture);
            texture.display();

            const sf::Texture& sfTexture = texture.getTexture();
            sf::Image image = sfTexture.copyToImage();

            QImage qtImage(
                image.getPixelsPtr(),
                image.getSize().x,
                image.getSize().y,
                QImage::Format_RGBA8888
                );

            QPixmap pixmap = QPixmap::fromImage(qtImage);
            painter.drawPixmap(0, 0, pixmap);

        } catch (const std::exception& e) {
            std::cout << "Rendering failed: " << e.what() << std::endl;
            painter.fillRect(rect(), Qt::red);
        }
    } else {
        painter.fillRect(rect(), Qt::blue);
    }

    // Отладочная информация
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 14));
    painter.drawText(10, 30, "3D Engine - Press ESC to exit");
    painter.drawText(10, 60, QString("Frame: %1").arg(m_frameCount));
}

void GameWindow::keyPressEvent(QKeyEvent* event) {
    std::cout << "Key PRESSED: " << event->key() << std::endl;

    if (event->key() == Qt::Key_Escape) {
        stopGame();
        hide();
        emit gameFinished();
        return;
    }

    QWidget::keyPressEvent(event);
}

// ДОБАВЛЯЕМ РЕАЛИЗАЦИЮ keyReleaseEvent
void GameWindow::keyReleaseEvent(QKeyEvent* event) {
    std::cout << "Key RELEASED: " << event->key() << std::endl;
    QWidget::keyReleaseEvent(event);
}

void GameWindow::showEvent(QShowEvent* event) {
    QWidget::showEvent(event);
    std::cout << "GameWindow is now visible" << std::endl;
}
