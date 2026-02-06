#pragma once
#include <QWidget>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

signals:
    void backToMenu();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onBackButtonClicked();
};
