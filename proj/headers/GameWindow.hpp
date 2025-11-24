#pragma once
#include "World.hpp"
#include <SFML/Window/Keyboard.hpp>

#include <QPainter>
#include <QKeyEvent>
#include <QShowEvent>
#include <QElapsedTimer>
#include <QWidget>
#include <QTimer>
#include <QDesktopWidget>
#include <QScreen>
#include <QApplication>

class GameWindow : public QWidget
{
    Q_OBJECT

private:
    QTimer *m_timer;
    bool m_initialized;
    int m_currentFPS = 0;

    sf::RenderTexture m_renderTexture;
    World m_world;
    QPixmap m_pixmap;

    void initializeSFML();
    void renderFrame();
    void handleSFMLEvents();

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    void startGame();
    void stopGame();

signals:
    void gameFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

private slots:
    void onUpdate();

};
