#include <../headers/EditorWindow.hpp>

EditorWindow::EditorWindow(QWidget *parent):QWidget(parent)
{
    mainLayout = new QHBoxLayout();

    initEditor();
    initToolsBar();

    mainLayout->addLayout(editorLayout, 4);
    mainLayout->addLayout(toolsLayout, 1);

    setLayout(mainLayout);
}

void EditorWindow::initEditor(){
    editorLayout = new QVBoxLayout();

    initMap();
    initButtonsBar();


    editorLayout->addLayout(mapLayout,7);
    editorLayout->addLayout(buttonsLayout,1);
}

void EditorWindow::initToolsBar(){
    toolsLayout = new QVBoxLayout();

    QLabel* tri = new QLabel("3");
    toolsLayout->addWidget(tri);

}

void EditorWindow::initMap() {
    mapLayout = new QHBoxLayout();

    QLabel* two = new QLabel("2");
    mapLayout->addWidget(two);
}

void EditorWindow::initButtonsBar(){
    buttonsLayout = new QHBoxLayout();
    QLabel* one = new QLabel("1");

    buttonsLayout->addWidget(one);
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
