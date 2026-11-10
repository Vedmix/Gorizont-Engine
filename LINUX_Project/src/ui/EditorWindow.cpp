#include <../headers/EditorWindow.hpp>

EditorWindow::EditorWindow(QWidget *parent):QWidget(parent)
{
    mainLayout = new QHBoxLayout();

    initEditor();
    initToolsBar();

    mainLayout->addLayout(editorLayout, 4);
    mainLayout->addLayout(toolsLayout, 1);

    setLayout(mainLayout);

    m_templatePath = saveTemplateMap();
    updateMinimap();

    showFullScreen();
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

    for(size_t i = 0; i < editorNames.size(); i++){
        QPushButton *button = new QPushButton(editorNames[i], this);
        button->setFixedSize(200, 50);
        toolsLayout->addWidget(button);
        switch(i){
        case 0:
            connect(button, &QPushButton::clicked, this, &EditorWindow::onAddCircleClicked);
            break;
        case 1:
            connect(button, &QPushButton::clicked, this, &EditorWindow::onAddWallClicked);
            break;
        }
    }

    toolsLayout->addStretch();

}

void EditorWindow::initMap() {
    mapLayout = new QHBoxLayout();
    m_canvas = new MapCanvas(this);
    mapLayout->addWidget(m_canvas);
}

void EditorWindow::initButtonsBar(){
    buttonsLayout = new QHBoxLayout();

    buttonsLayout->setAlignment(Qt::AlignCenter);

    for(size_t i = 0; i < buttonNames.size(); i++){
        QPushButton *button = new QPushButton(buttonNames[i], this);
        button->setFixedSize(200, 50);
        buttonsLayout->addWidget(button);
        switch(i){
        case 0:
            connect(button, &QPushButton::clicked, this, &EditorWindow::onTestButtonClicked);
            break;
        case 1:
            connect(button, &QPushButton::clicked, this, &EditorWindow::onSaveButtonClicked);
            break;
        case 2:
            connect(button, &QPushButton::clicked, this, &EditorWindow::onBackButtonClicked);
            break;
        }
    }
}



void EditorWindow::updateMinimap(){
    if(m_templatePath.isEmpty()) return;

    auto& s = AppSettings::instance();

    m_previewWorld.clearMap();
    m_previewWorld.setMapPath(m_templatePath.toStdString());
    m_previewWorld.loadMapFromXML();

    sf::RenderTexture tex({(unsigned)s.screenWidth(), (unsigned)s.screenHeight()});
    m_previewWorld.renderMapPreview(tex);
    tex.display();

    sf::Image img = tex.getTexture().copyToImage();
    QPixmap bg = QPixmap::fromImage(QImage(img.getPixelsPtr(), img.getSize().x, img.getSize().y, QImage::Format_RGBA8888).copy());

    m_canvas->setBackground(bg, QSize(s.screenWidth(), s.screenHeight()));
}

// Подгон миникарты под размер label
bool EditorWindow::eventFilter(QObject* obj, QEvent* event){
    if(obj == m_mapLabel && event->type() == QEvent::Resize && !m_mapPixmap.isNull()){
        m_mapLabel->setPixmap(m_mapPixmap.scaled(m_mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    return QWidget::eventFilter(obj, event);
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
    QFile file("maps/template_map.xml");
    if (!file.remove()) {
        qDebug() << "Cannot delete template map";
    }
    emit backToMenu();
    close();
}

void EditorWindow::onTestButtonClicked(){
    m_templatePath = saveTemplateMap();   // сохраняем, чтобы игра увидела текущие круги
    if(m_templatePath.isEmpty()) return;

    m_game = new GameWindow(nullptr, QDir::current().absoluteFilePath(m_templatePath));
    m_game->setAttribute(Qt::WA_DeleteOnClose);
    m_game->setWindowTitle("Тест карты");
    m_game->resize(AppSettings::instance().screenWidth(), AppSettings::instance().screenHeight());

    connect(m_game, &GameWindow::gameFinished, this, &EditorWindow::onTestFinished);

    hide();
    m_game->showFullScreen();
    m_game->activateWindow();
    m_game->raise();
}

void EditorWindow::onTestFinished(){
    showFullScreen();
    activateWindow();
    raise();

    m_game->close();
    m_game = nullptr;
}

QString EditorWindow::saveTemplateMap(){

    QDir().mkpath("maps");
    const QString path = "maps/template_map.xml";

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        qDebug() << "Cannot write template map:" << path;
        return QString();
    }

    QTextStream out(&file);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<map>\n    <walls>\n";
    for(const auto& obj : m_canvas->objects()){
        if(obj->getObjectType() == ObjectType::POLYGON){
            out << obj->toXml();
        }
    }
    out << "    </walls>\n    <circles>\n";
    for(const auto& obj : m_canvas->objects()){
        if(obj->getObjectType() == ObjectType::CIRCLE){
            out << obj->toXml();
        }
    }
    out << "    </circles>\n</map>\n";

    return path;
}

void EditorWindow::onAddCircleClicked(){ m_canvas->addCircle(); }

void EditorWindow::onSaveButtonClicked(){ m_templatePath = saveTemplateMap(); }

void EditorWindow::onAddWallClicked(){ m_canvas->addWall(); }
