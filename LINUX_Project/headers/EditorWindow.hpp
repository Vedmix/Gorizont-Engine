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
#include <QPixmap>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include "World.hpp"
#include "GameWindow.hpp"

class EditorWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EditorWindow  (QWidget *parent = nullptr);
signals:
    void backToMenu();
private slots:
    void onBackButtonClicked();
    void onTestButtonClicked();
    void onTestFinished();

private:
    World m_previewWorld;
    QLabel* m_mapLabel = nullptr;
    QPixmap m_mapPixmap;
    QString m_templatePath;

    GameWindow* m_game = nullptr;

    QHBoxLayout* mainLayout;
    QHBoxLayout* mapLayout;
    QHBoxLayout* buttonsLayout;
    QVBoxLayout* toolsLayout;
    QVBoxLayout* editorLayout;

    QString createTemplateMap();

    void initEditor();
    void initMap();
    void initToolsBar();
    void initButtonsBar();
    void updateMinimap();
    bool eventFilter(QObject* obj, QEvent* event) override;
    void initButtons();
    void keyPressEvent(QKeyEvent *event) override;

    const std::vector<QString> buttonNames = {
        "Протестировать",
        "Выход"
    };    
};
