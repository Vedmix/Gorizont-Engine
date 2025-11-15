#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QPixmap>
#include <SFML/Graphics.hpp>
#include "../headers/World.hpp"

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
    void initializeSFML();  // ДОБАВИТЬ ЭТО
    void renderFrame();     // ДОБАВИТЬ ЭТО
    void handleSFMLEvents();

    QTimer *m_timer;
    sf::RenderTexture m_renderTexture;
    bool m_initialized;
    World m_world;
    QPixmap m_pixmap;
};

#endif // GAMEWINDOW_H
