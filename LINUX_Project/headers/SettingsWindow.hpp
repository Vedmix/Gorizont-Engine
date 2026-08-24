#pragma once
#include "AppSettings.hpp"

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
#include <cmath>
#include <QPropertyAnimation>
#include <QTimer>
#include <QStyle>
#include <QStyleFactory>
#include <QFileInfo>
#include <QGroupBox>
#include <QComboBox>
#include <QFileDialog>

class SettingsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsWindow (QWidget *parent = nullptr);

signals:
    void backToMenu();
private slots:
    void onBackButtonClicked();
    void onSaveButtonClicked();
    void onDefaultButtonClicked();
    void onSelectMapClicked();

private:
    QVBoxLayout* mainLayout = nullptr;
    QHBoxLayout* settingsLayout = nullptr;
    QHBoxLayout* buttonsLayout = nullptr;

    QVBoxLayout* gameSettingsLayout = nullptr;
    QVBoxLayout* interfaceSettingsLayout = nullptr;

    QGroupBox* gameSettingsGroup = nullptr;
    QGroupBox* interfaceSettingsGroup = nullptr;

    QVBoxLayout* sliderLayout = nullptr;
    QHBoxLayout* mapLayout = nullptr;
    QHBoxLayout* radioLayout = nullptr;
    QHBoxLayout* themeLayout = nullptr;

    std::vector<QSlider*> gameSliders;
    std::vector<QLabel*> sliderValueLabels;
    QComboBox* mapComboBox = nullptr;
    QComboBox* themeComboBox = nullptr;

    QWidget* notificationWidget = nullptr;
    QLabel* notificationLabel = nullptr;
    QPropertyAnimation* notificationAnimation = nullptr;
    QTimer* notificationTimer = nullptr;

    void initUI();
    void initGameSettings();
    void initInterfaceSettings();

    void initRadioButton();
    void initMapSelector();
    void initThemeSelector();
    void initSliders();
    void initButtons();

    void initNotification();
    void showNotification(const QString& text, bool success = true);
    void hideNotification();

    void keyPressEvent(QKeyEvent *event) override;

    void chooseTheme(const QString& theme);

    int sliderWidth = 300;

    const std::vector<QString> sliderNames = {
        "FOV",
        "Graphics",
        "Distance",
        "Speed"
    };

    std::vector<std::pair<int, int>> slidersRanges = {
        {30, 120},   // FOV
        {100, 3840}, // Количество лучей
        {100, 2000}, // Дальность
        {50, 500}    // Скорость
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