#pragma once
#include <QWidget>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

signals:
    void backToMenu();

protected:

    void initSliders();
    void initRadioButtons();
    void initButtons();

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void onBackButtonClicked();

private:
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    QHBoxLayout *radioButtonsLayout = new QHBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    const std::vector<QString> sliderNames = {
        "FOV",
        "Graphics",
        "Distance",
        "Speed"
    };

    const std::vector<QString> radioButtonNames = {
        "DRUGS MOD"
    };

    const std::vector<QString> buttonNames = {
        "Сохранить",
        "По умолчанию",
        "Выход"
    };
};
