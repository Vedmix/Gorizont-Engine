#include "../headers/GameWindow.hpp"

GameWindow::GameWindow(QWidget *parent): QWidget(parent), m_timer(new QTimer(this)), m_initialized(false), m_world(), m_currentFPS(0)
{
    connect(m_timer, &QTimer::timeout, this, &GameWindow::onUpdate);
    resize(SCREEN_WIDTH, SCREEN_HEIGHT);
    setFocusPolicy(Qt::StrongFocus);
}

GameWindow::~GameWindow()
{
    stopGame();
}

void GameWindow::startGame()
{
    if(!m_initialized) {
        initializeSFML();
    }

    if(m_timer && !m_timer->isActive()) {
        m_timer->start(16); // ~60 FPS
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

    m_initialized = true;
    m_world.loadMapFromXML();
}

void GameWindow::renderFrame()
{
    if(!m_initialized){
        return;
    }

    // Рендерим в текстуру через World
    m_world.renderToTexture(m_renderTexture);

    // Копируем текстуру в QPixmap
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
    if(!m_initialized){
        return;
    }

    static QElapsedTimer deltaTimer;
    static bool firstCall = true;

    if(firstCall){
        deltaTimer.start();
        firstCall = false;
        return;
    }

    double deltaTime = deltaTimer.restart() / 1000.0;
    m_world.update(deltaTime);
}

void GameWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    if(!m_pixmap.isNull()){
        painter.drawPixmap(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, m_pixmap);

        // Отладочная информация
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.setPen(Qt::green);
        painter.drawText(SCREEN_WIDTH - 200, 40, QString("FPS: %1").arg(m_currentFPS));
    } else{
        painter.fillRect(rect(), Qt::black);
        painter.setPen(Qt::white);
        painter.drawText(rect(), Qt::AlignCenter, "Initializing SFML render texture...");
    }
}

void GameWindow::keyPressEvent(QKeyEvent* event)
{
    if(!m_initialized){
        QWidget::keyPressEvent(event);
        return;
    }

    if(event->key() == Qt::Key_Escape){
        emit gameFinished();
        return;
    }

    QWidget::keyPressEvent(event);
}

void GameWindow::showEvent(QShowEvent* event)
{
    Q_UNUSED(event);

    if(!m_initialized){
        initializeSFML();
    }

    startGame();
}

void GameWindow::onUpdate()
{
    if(!m_initialized){
        return;
    }

    handleSFMLEvents();
    renderFrame();

    static QElapsedTimer fpsTimer;
    static int frameCount = 0;

    if(frameCount == 0){
        fpsTimer.start();
    }

    frameCount++;

    if(fpsTimer.elapsed() >= 1000){
        m_currentFPS = frameCount;
        frameCount = 0;
        fpsTimer.restart();
    }
}
