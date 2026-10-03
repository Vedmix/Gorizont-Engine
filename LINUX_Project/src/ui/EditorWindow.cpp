#include <../headers/EditorWindow.hpp>

EditorWindow::EditorWindow(QWidget *parent):QWidget(parent)
{
    initButtons();

    mainLayout->addLayout(buttonsLayout);
    mainLayout->addStretch();

    setLayout(mainLayout);
}
void EditorWindow::initButtons(){

    buttonsLayout->setAlignment(Qt::AlignCenter);

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);
        buttonsLayout->addWidget(button);
        switch(i){
        case 0:
            connect(button, &QPushButton::clicked, this, &EditorWindow::onBackButtonClicked);
            break;
        }
    }
}

void EditorWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Escape){
        onBackButtonClicked();
    }else{
        QWidget::keyPressEvent(event);
    }
}

void EditorWindow::onBackButtonClicked(){
    emit backToMenu();
    close();
}
