#include "GameWindow.h"


GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent),
    m_initialized(false),
    m_world()
{
    setMinimumSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &GameWindow::onUpdate);

    QTimer::singleShot(100, this, &GameWindow::initializeSFML);
}

GameWindow::~GameWindow()
{
    if (m_initialized) {
        m_timer->stop();
    }
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event)

    QPainter painter(this);

    if (m_initialized && !m_pixmap.isNull()) {
        painter.drawPixmap(0, 0, m_pixmap);
    } else {
        painter.fillRect(rect(), QColor(30, 30, 50));
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Initializing Game...");
    }
}

void GameWindow::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    if (m_initialized) {
        m_renderTexture.create(width(), height());
    }
}

void GameWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    qDebug() << "GameWindow shown";
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) {
        stopGame();
        hide();
        emit gameFinished();
        return;
    }
    switch(event->key()) {
    case Qt::Key_W:
    case Qt::Key_A:
    case Qt::Key_S:
    case Qt::Key_D:
    case Qt::Key_Left:
    case Qt::Key_Right:
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void GameWindow::initializeSFML()
{
    if (m_initialized) return;


    if (!m_renderTexture.create(width(), height())) {
        throw std::runtime_error("Failed to create render texture");
    }

    m_initialized = true;
    m_timer->start(16); // ~60 FPS


}

// РЕАЛИЗАЦИЯ renderFrame
void GameWindow::renderFrame()
{
    if (!m_initialized) return;
    m_world.renderToTexture(m_renderTexture);
    m_renderTexture.display();

    // Конвертируем в QPixmap
    const sf::Texture& texture = m_renderTexture.getTexture();
    sf::Image image = texture.copyToImage();

    QImage qtImage(image.getPixelsPtr(), image.getSize().x, image.getSize().y, QImage::Format_RGBA8888);
    m_pixmap = QPixmap::fromImage(qtImage.copy());

}

void GameWindow::handleSFMLEvents()
{
    // Обработка событий SFML, если необходимо
}

void GameWindow::startGame()
{
    if (!m_initialized) {
        initializeSFML();
    }

    setFocus();
    qDebug() << "Game started";
}

void GameWindow::stopGame()
{
    m_timer->stop();
    qDebug() << "Game stopped";
}

void GameWindow::onUpdate()
{
    if (m_initialized) {
        // Обновляем игровую логику
        double deltaTime = 16.0 / 1000.0;
        m_world.update(deltaTime);

        // Рендерим кадр
        renderFrame();

        if (!m_pixmap.isNull()) {
            update();
        }
    }
}
