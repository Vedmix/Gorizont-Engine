#pragma once
#include <QWidget>
#include <QString>
#include <QHBoxLayout>
#include <QWidget>
#include <QGuiApplication>
#include <QPushButton>
#include <QKeyEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>

class CreditsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CreditsWindow  (QWidget *parent = nullptr);
signals:
    void backToMenu();
private slots:
    void onBackButtonClicked();
private:
    void initButtons();

    void keyPressEvent(QKeyEvent *event) override;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    const std::vector<QString> buttonNames = {
        "Выход"
    };
};
