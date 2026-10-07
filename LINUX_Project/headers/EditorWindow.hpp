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

class EditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWindow  (QWidget *parent = nullptr);
signals:
    void backToMenu();
private slots:
    void onBackButtonClicked();
private:
    void initEditor();
    void initMap();
    void initToolsBar();
    void initButtonsBar();

    QHBoxLayout* mainLayout;
    QHBoxLayout* mapLayout;
    QHBoxLayout* buttonsLayout;
    QVBoxLayout* toolsLayout;
    QVBoxLayout* editorLayout;

    void initButtons();
    void keyPressEvent(QKeyEvent *event) override;


    const std::vector<QString> buttonNames = {
        "Выход"
    };
};
