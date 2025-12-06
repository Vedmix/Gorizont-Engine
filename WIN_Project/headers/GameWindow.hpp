#pragma once
#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QPainter>
#include <QKeyEvent>
#include <SFML/Graphics.hpp>
#include "../headers/World.hpp"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    virtual ~GameWindow();

    void startGame();
    void stopGame();

signals:
    void gameFinished();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void showEvent(QShowEvent* event) override;

private slots:
    void onUpdate();

private:
    void initializeSFML();
    void renderFrame();
    void handleSFMLEvents();

    QTimer* m_timer;
    bool m_initialized;
    World m_world;
    sf::RenderTexture m_renderTexture;
    QPixmap m_pixmap;
    int m_currentFPS;
};
