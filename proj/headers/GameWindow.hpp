#pragma once

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include <QShowEvent>
#include <cmath>
#include "WorldAdapter.hpp"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget* parent = nullptr);
    ~GameWindow(); // ДОБАВЛЯЕМ ДЕСТРУКТОР

    void startGame();
    void stopGame();

signals:
    void gameFinished();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override; // ДОБАВЛЯЕМ
    void showEvent(QShowEvent* event) override;

private slots:
    void onUpdate();

private:
    WorldAdapter m_worldAdapter;
    QTimer* m_timer;
    int m_frameCount;
};
