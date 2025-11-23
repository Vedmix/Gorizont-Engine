#include "../headers/SimpleGameWindow.hpp"
#include <iostream>
#include <QApplication>

SimpleGameWindow::SimpleGameWindow(QWidget* parent)
    : QWidget(parent),
    m_initialized(true),
    m_timer(new QTimer(this)),
    m_frameCount(0)
{
    setMinimumSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);

    connect(m_timer, &QTimer::timeout, this, &SimpleGameWindow::onUpdate);
    
    // Устанавливаем черный фон
    setStyleSheet("background-color: black;");
    
    std::cout << "SimpleGameWindow created" << std::endl;
}

SimpleGameWindow::~SimpleGameWindow()
{
    if (m_initialized) {
        m_timer->stop();
    }
}

void SimpleGameWindow::startGame()
{
    std::cout << "Starting game..." << std::endl;
    m_timer->start(16); // ~60 FPS
    setFocus();
}

void SimpleGameWindow::stopGame()
{
    m_timer->stop();
    std::cout << "Game stopped" << std::endl;
}

void SimpleGameWindow::onUpdate()
{
    if (m_initialized) {
        double deltaTime = 16.0 / 1000.0;
        m_worldAdapter.update(deltaTime);
        m_worldAdapter.handleEvents();
        
        m_frameCount++;
        update(); // Запрашиваем перерисовку
        
        if (m_frameCount % 60 == 0) {
            std::cout << "Frame " << m_frameCount << " updated" << std::endl;
        }
    }
}

void SimpleGameWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Очищаем черным
    painter.fillRect(rect(), Qt::black);
    
    // Рисуем тестовую графику
    drawTestGraphics(painter);
    
    // Отладочная информация
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 16));
    painter.drawText(10, 30, "Simple Game Window - Press ESC to exit");
    painter.drawText(10, 60, QString("Frame: %1").arg(m_frameCount));
}

void SimpleGameWindow::drawTestGraphics(QPainter& painter)
{
    // 1. Синий прямоугольник
    painter.fillRect(100, 100, 400, 300, QColor(0, 0, 255));
    
    // 2. Красный круг
    painter.setBrush(QBrush(Qt::red));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(200, 200, 150, 150);
    
    // 3. Зеленая линия
    painter.setPen(QPen(Qt::green, 5));
    painter.drawLine(50, 50, 500, 400);
    
    // 4. Желтый текст
    painter.setPen(Qt::yellow);
    painter.setFont(QFont("Arial", 24));
    painter.drawText(150, 500, "3D ENGINE TEST");
    
    // 5. Мини-карта в углу
    painter.setBrush(QBrush(QColor(0, 0, 0, 180)));
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRect(width() - 220, 20, 200, 200);
    
    // Объекты на мини-карте
    painter.setBrush(QBrush(Qt::red));
    painter.setPen(Qt::NoPen);
    painter.drawRect(width() - 210, 30, 10, 10);
    painter.drawRect(width() - 190, 50, 10, 10);
    painter.drawRect(width() - 170, 70, 10, 10);
    
    // Камера на мини-карте
    painter.setBrush(QBrush(Qt::green));
    QPointF points[3] = {
        QPointF(width() - 150, 100),
        QPointF(width() - 160, 120),
        QPointF(width() - 140, 120)
    };
    painter.drawPolygon(points, 3);
}

void SimpleGameWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        stopGame();
        hide();
        emit gameFinished();
        return;
    }

    QWidget::keyPressEvent(event);
}