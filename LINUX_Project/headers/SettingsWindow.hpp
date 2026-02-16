#pragma once
#include <QWidget>
#include <QKeyEvent>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>
#include <QCloseEvent> // Добавлено

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
    void closeEvent(QCloseEvent *event) override; // Добавлено

private slots:
    void onBackButtonClicked();

private:
    QString getGraphicsText(int value) const;

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    QHBoxLayout *radioButtonsLayout = new QHBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();

    QSlider* fovSlider = nullptr;
    QSlider* graphicsSlider = nullptr;
    QSlider* distanceSlider = nullptr;
    QSlider* speedSlider = nullptr;
    QRadioButton* drugsRadioButton = nullptr;

    const std::vector<QString> sliderNames = {
        "FOV",
        "Graphics (rays)",
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
