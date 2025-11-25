#pragma once

#include <QWidget>
#include <QTimer>
#include <QKeyEvent>
#include <QPainter>
#include "WorldAdapter.hpp"

class SimpleGameWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SimpleGameWindow(QWidget* parent = nullptr);
    ~SimpleGameWindow();

    void startGame();
    void stopGame();

signals:
    void gameFinished();

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onUpdate();

private:
    void drawTestGraphics(QPainter& painter);

    bool m_initialized;
    WorldAdapter m_worldAdapter;
    QTimer* m_timer;
    int m_frameCount;
};