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

    QVBoxLayout *sliderLayout = new QVBoxLayout();
    QHBoxLayout *mapLayout = new QHBoxLayout();
    QHBoxLayout *radioLayout = new QHBoxLayout();
    QHBoxLayout *themeLayout = new QHBoxLayout();

    void initUI();
    void initGameSettings();
    void initInterfaceSettings();
    void initRadioButton();
    void initMapSelector();
    void initSliders();
    void initButtons();
    void initNotification();

    void keyPressEvent(QKeyEvent *event) override;
    void showNotification(const QString& text, bool success = true);
    void hideNotification();
    void onThemeChanged(const QString& theme);
    void chooseTheme(const QString& theme);

    QVBoxLayout* m_mainLayout;
    QHBoxLayout* m_contentLayout;
    QHBoxLayout* m_buttonsLayout;

     QRadioButton* m_drugsRadioButton;

    QWidget* notificationWidget = nullptr;
    QLabel* notificationLabel = nullptr;
    QPropertyAnimation* notificationAnimation = nullptr;
    QTimer* notificationTimer = nullptr;

    // Колонка "Игра"
    QGroupBox* m_gameGroup;
    QVBoxLayout* m_gameLayout;
    std::vector<QSlider*> m_gameSliders;
    std::vector<QLabel*> m_sliderValueLabels;
    QComboBox* m_mapComboBox;
    QPushButton* m_selectMapButton;

    // Колонка "Интерфейс"
    QGroupBox* m_interfaceGroup;
    QVBoxLayout* m_interfaceLayout;
    QComboBox* m_themeComboBox;

    // Уведомления
    QWidget* m_notificationWidget = nullptr;
    QLabel* m_notificationLabel = nullptr;
    QPropertyAnimation* m_notificationAnimation = nullptr;
    QTimer* m_notificationTimer = nullptr;

    // Кнопки

    int sliderWidth = 300;

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