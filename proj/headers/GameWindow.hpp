#pragma once

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QShowEvent>
#include <cmath>
#include "WorldAdapter.hpp"
#include "settings.hpp"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = nullptr);
    ~GameWindow();

    void startGame();
    void stopGame();

signals:
    void gameFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void showEvent(QShowEvent* event) override;

private slots:
    void onUpdate();

private:
    void initializeSFML();
    void renderFrame();
    void handleSFMLEvents();

    QTimer *m_timer;
    sf::RenderTexture m_renderTexture;
    bool m_initialized;
    World m_world;
    QPixmap m_pixmap;
    WorldAdapter* worldAdapter; // Добавлено
};
