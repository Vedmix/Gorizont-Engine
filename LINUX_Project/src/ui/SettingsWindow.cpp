#include "../headers/SettingsWindow.hpp"

SettingsWindow::SettingsWindow(QWidget *parent): QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addStretch();

    QPushButton *backButton = new QPushButton("Выход в меню", this);
    backButton->setFixedSize(200, 50);
    backButton->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    border: none;"
        "    color: white;"
        "    font-size: 14px;"
        "    border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        );

    connect(backButton, &QPushButton::clicked, this, &SettingsWindow::onBackButtonClicked);

    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void SettingsWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        onBackButtonClicked();
    }else{
        QWidget::keyPressEvent(event);
    }
}

void SettingsWindow::onBackButtonClicked()
{
    emit backToMenu();
    close();
}
