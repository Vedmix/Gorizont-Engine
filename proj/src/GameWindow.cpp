#include "../headers/GameWindow.hpp"
#include "../headers/WorldAdapter.hpp"

#include <QDebug>
#include <QPainter>
#include <QResizeEvent>
#include <QKeyEvent>
#include <QShowEvent>
#include <QElapsedTimer>

// Для SFML input
#include <SFML/Window/Keyboard.hpp>


GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , m_timer(new QTimer(this))
    , m_initialized(false)
    , m_world()
    , worldAdapter(nullptr)
{
    connect(m_timer, &QTimer::timeout, this, &GameWindow::onUpdate);
    setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
}

GameWindow::~GameWindow()
{
    stopGame();
    delete worldAdapter;
}

void GameWindow::startGame()
{
    if (!m_initialized) {
        initializeSFML();
    }

    if (m_timer && !m_timer->isActive()) {
        m_timer->start(33); // ~60 FPS
    }
}

void GameWindow::stopGame()
{
    if (m_timer && m_timer->isActive()) {
        m_timer->stop();
    }
}

void GameWindow::initializeSFML()
{
    if (!m_renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        qDebug() << "Failed to create render texture";
        return;
    }

    worldAdapter = new WorldAdapter(m_world.getMap(), m_world.getCamera(), m_world);
    m_initialized = true;

    // Настройка мира
    m_world.setMapOption2();

    qDebug() << "SFML initialized successfully";
}

void GameWindow::renderFrame()
{
    if (!m_initialized || !worldAdapter) return;

    m_renderTexture.clear(sf::Color::Black);
    worldAdapter->renderWorld(m_renderTexture);
    m_renderTexture.display();

    const sf::Texture& texture = m_renderTexture.getTexture();
    sf::Image image = texture.copyToImage();
    m_pixmap = QPixmap::fromImage(QImage(
        image.getPixelsPtr(),
        image.getSize().x,
        image.getSize().y,
        QImage::Format_RGBA8888
        ));

    update();
}

void GameWindow::handleSFMLEvents()
{
    // Используем оригинальный метод управления камеры
    static QElapsedTimer deltaTimer;
    static bool firstCall = true;

    if (firstCall) {
        deltaTimer.start();
        firstCall = false;
        return;
    }

    double deltaTime = deltaTimer.restart() / 1000.0;

    // Вызываем оригинальный метод управления
    m_world.getCamera().moveWithKeyboard(deltaTime);
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_pixmap.isNull()) {
        painter.drawPixmap(rect(), m_pixmap, m_pixmap.rect());
    } else {
        painter.fillRect(rect(), Qt::black);
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Loading...");
    }
}

void GameWindow::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    // Фиксированный размер
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if (!m_initialized) return;

    if (event->key() == Qt::Key_Escape) {
        emit gameFinished();
    }

    QWidget::keyPressEvent(event);
}

void GameWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);

    if (!m_initialized) {
        initializeSFML();
    }

    startGame();
}

void GameWindow::onUpdate()
{
    if (!m_initialized) return;

    // ОБРАБОТКА УПРАВЛЕНИЯ КАЖДЫЙ КАДР
    handleSFMLEvents();

    // РЕНДЕРИНГ ТОЛЬКО КАЖДЫЙ ВТОРОЙ КАДР для увеличения FPS
    static int frameCounter = 0;
    frameCounter++;

    if (frameCounter % 2 == 0) { // Рендерим каждый второй кадр
        renderFrame();
    } else {
        // Пропускаем рендеринг, но обновляем окно
        update();
    }

    // FPS counter
    static QElapsedTimer frameTimer;
    static int frameCount = 0;

    if (frameCount == 0) {
        frameTimer.start();
    }

    frameCount++;

    if (frameTimer.elapsed() >= 1000) {
        setWindowTitle(QString("Game - FPS: %1").arg(frameCount));
        frameCount = 0;
    }
}
