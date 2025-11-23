#include "../headers/GameWindow.hpp"


GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , m_timer(new QTimer(this))
    , m_initialized(false)
    , m_world()
    , worldAdapter(nullptr)
{
    connect(m_timer, &QTimer::timeout, this, &GameWindow::onUpdate);
    resize(SCREEN_WIDTH, SCREEN_HEIGHT);
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
        m_timer->start(8);
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
        return;
    }

    worldAdapter = new WorldAdapter(m_world.getMap(), m_world.getCamera(), m_world);
    m_initialized = true;
    m_world.setMapOption2();
}

void GameWindow::renderFrame()
{
    if (!m_initialized || !worldAdapter) return;

    QSize widgetSize = size();

    if (m_renderTexture.getSize().x != SCREEN_WIDTH || m_renderTexture.getSize().y != SCREEN_HEIGHT) {
        m_renderTexture.create(SCREEN_WIDTH, SCREEN_HEIGHT);
    }

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
    static QElapsedTimer deltaTimer;
    static bool firstCall = true;

    if (firstCall) {
        deltaTimer.start();
        firstCall = false;
        return;
    }

    double deltaTime = deltaTimer.restart() / 1000.0;
    m_world.getCamera().moveWithKeyboard(deltaTime);
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if (!m_pixmap.isNull()) {
        painter.drawPixmap(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, m_pixmap);

        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.setPen(Qt::green);
        painter.drawText(SCREEN_WIDTH - 150, 40, QString("FPS: %1").arg(m_currentFPS));
    } else {
        painter.fillRect(rect(), Qt::black);
    }
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

    handleSFMLEvents();

    static int frameCounter = 0;
    frameCounter++;

    if (frameCounter % 2 == 0) {
        renderFrame();

        static QElapsedTimer frameTimer;
        static int frameCount = 0;

        if (frameCount == 0) {
            frameTimer.start();
        }

        frameCount++;

        if (frameTimer.elapsed() >= 1000) {
            m_currentFPS = frameCount * 2;
            frameCount = 0;
            frameTimer.restart();
        }

        // Всегда обновляем после renderFrame
        update();
    }
}
