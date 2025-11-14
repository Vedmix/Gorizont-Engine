#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include "sfml_widget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

private slots:
    void onColorChanged();
    void onSizeChanged(int value);
    void onToggleAnimation();

private:
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QHBoxLayout *controlLayout;
    QLabel *titleLabel;
    QLabel *sizeLabel;
    QPushButton *colorButton;
    QPushButton *animationButton;
    QSlider *sizeSlider;
    SFMLWidget *sfmlWidget;
    QTimer *animationTimer;
    bool animationRunning;
};

#endif // MAIN_WINDOW_H