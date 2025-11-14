#include "sfml_widget.h"
#include <QPainter>
#include <QPaintEvent>
#include <cmath>
#include <QDebug>

SFMLWidget::SFMLWidget(QWidget *parent)
    : QWidget(parent),
    circleRadius(300.0f),
    circleColor(sf::Color::Green),
    animationAngle(0.0f),
    initialized(false) {

    setMinimumSize(400, 300);

    renderTimer = new QTimer(this);
    connect(renderTimer, &QTimer::timeout, this, [this]() {
        if (initialized) {
            renderFrame();
            update();
        }
    });

    qDebug() << "SFMLWidget created";
}

SFMLWidget::~SFMLWidget() {
    if (initialized) {
        renderTimer->stop();
    }
}

void SFMLWidget::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event)
    
    QPainter painter(this);
    
    if (initialized && !pixmap.isNull()) {
        painter.drawPixmap(0, 0, pixmap);
    } else {
        painter.fillRect(rect(), QColor(30, 30, 50));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "SFML Initializing...");
    }
}

void SFMLWidget::resizeEvent(QResizeEvent* event) {
    QWidget::resizeEvent(event);
    if (initialized) {
        // Пересоздаем текстуру при изменении размера
        renderTexture.create(width(), height());
    }
}

void SFMLWidget::initializeSFML() {
    if (initialized) return;

    try {
        qDebug() << "Initializing SFML RenderTexture...";

        // Создаем текстуру для рендеринга
        if (!renderTexture.create(width(), height())) {
            throw std::runtime_error("Failed to create render texture");
        }

        // Настройка графики
        circle.setRadius(circleRadius);
        circle.setFillColor(circleColor);
        circle.setOutlineColor(sf::Color::Red);
        circle.setOutlineThickness(2.0f);
        circle.setOrigin(circleRadius, circleRadius);

        initialized = true;
        renderTimer->start(16);
        
        qDebug() << "SFML RenderTexture initialized successfully!";

    } catch (const std::exception& e) {
        qCritical() << "SFML init failed:" << e.what();
        
        QPainter painter(this);
        painter.fillRect(rect(), QColor(80, 30, 30));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, 
                         QString("SFML Error: %1").arg(e.what()));
    }
}

void SFMLWidget::renderFrame() {
    if (!initialized) return;

    try {
        // Очистка
        renderTexture.clear(sf::Color(40, 40, 60));

        // Анимация
        sf::Vector2u textureSize = renderTexture.getSize();
        float posX = textureSize.x / 2.0f + sin(animationAngle) * 100.0f;
        float posY = textureSize.y / 2.0f + cos(animationAngle) * 50.0f;

        circle.setPosition(posX, posY);
        renderTexture.draw(circle);
        renderTexture.display();

        // Конвертируем SFML текстуру в QPixmap
        const sf::Texture& texture = renderTexture.getTexture();
        sf::Image image = texture.copyToImage();
        
        QImage qtImage(image.getPixelsPtr(), image.getSize().x, image.getSize().y, QImage::Format_RGBA8888);
        pixmap = QPixmap::fromImage(qtImage.copy());

    } catch (const std::exception& e) {
        qWarning() << "Render error:" << e.what();
    }
}

void SFMLWidget::setCircleColor(int r, int g, int b) {
    circleColor = sf::Color(r, g, b);
    if (initialized) {
        circle.setFillColor(circleColor);
    }
}

void SFMLWidget::setCircleRadius(float radius) {
    circleRadius = radius;
    if (initialized) {
        circle.setRadius(radius);
        circle.setOrigin(radius, radius);
    }
}

void SFMLWidget::updateAnimation() {
    animationAngle += 0.05f;
    if (animationAngle > 2 * 3.14159f) {
        animationAngle = 0.0f;
    }
    
    if (initialized) {
        renderFrame();
        update();
    }
}