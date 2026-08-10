#pragma once
#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QPainter>
#include <QKeyEvent>
#include <SFML/Graphics.hpp>
#include "World.hpp"
#include "AppSettings.hpp"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    virtual ~GameWindow();

    void startGame();
    void updateWorldSettings();
    void stopGame();

signals:
    void gameFinished();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void closeEvent(QCloseEvent* event) override;

private slots:
    void onUpdate();

private:
    void initializeSFML();
    void renderFrame();
    void handleSFMLEvents();
    void recreateWorld();

    QTimer* m_timer;
    bool m_initialized;
    std::unique_ptr<World> m_world;
    sf::RenderTexture m_renderTexture;
    QPixmap m_pixmap;
    int m_currentFPS;
};
