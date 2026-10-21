#include <../headers/EditorWindow.hpp>

EditorWindow::EditorWindow(QWidget *parent):QWidget(parent)
{
    mainLayout = new QHBoxLayout();

    initEditor();
    initToolsBar();

    mainLayout->addLayout(editorLayout, 4);
    mainLayout->addLayout(toolsLayout, 1);

    setLayout(mainLayout);

    m_templatePath = createTemplateMap();
    updateMinimap();
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

}

void EditorWindow::initMap() {
    mapLayout = new QHBoxLayout();

    m_mapLabel = new QLabel();
    m_mapLabel->setAlignment(Qt::AlignCenter);
    m_mapLabel->setMinimumSize(1, 1);
    m_mapLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_mapLabel->installEventFilter(this);
    mapLayout->addWidget(m_mapLabel);
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
            connect(button, &QPushButton::clicked, this, &EditorWindow::onBackButtonClicked);
            break;
        }
    }
}

QString EditorWindow::createTemplateMap(){
    auto& settings = AppSettings::instance();
    const int  wall_size = 30;

    QDir().mkpath("maps");
    const QString path = "maps/template_map.xml";

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        qDebug() << "Cannot write template map:" << path;
        return QString();
    }

    QTextStream out(&file);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<map>\n    <walls>\n";
    auto wall = [&](int x, int y, int w, int h){
        out << "        <wall x=\"" << x << "\" y=\"" << y << "\" width=\"" << w
            << "\" height=\"" << h << "\" color=\"0x808080FF\"/>\n";
    };
    wall(0, 0, settings.screenWidth(), wall_size);
    wall(0, settings.screenHeight() - wall_size, settings.screenWidth(), wall_size);
    wall(0, 0, wall_size, settings.screenHeight() - wall_size);
    wall(settings.screenWidth() - wall_size, 0, wall_size, settings.screenHeight() - wall_size);
    out << "    </walls>\n</map>\n";

    return path;
}

void EditorWindow::updateMinimap(){
    if(m_templatePath.isEmpty()) return;

    auto& settings = AppSettings::instance();

    m_previewWorld.clearMap();
    m_previewWorld.setMapPath(m_templatePath.toStdString());
    m_previewWorld.loadMapFromXML();

    sf::RenderTexture tex({(unsigned)settings.screenWidth(), (unsigned)settings.screenHeight()});
    tex.clear(sf::Color(30, 30, 30));
    m_previewWorld.renderMapPreview(tex);
    tex.display();

    sf::Image img = tex.getTexture().copyToImage();
    m_mapPixmap = QPixmap::fromImage(QImage(img.getPixelsPtr(), img.getSize().x, img.getSize().y,QImage::Format_RGBA8888).copy());

    m_mapLabel->setPixmap(m_mapPixmap.scaled(m_mapLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
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

    m_game = new GameWindow(nullptr, QDir::current().absoluteFilePath(m_templatePath));
    m_game->setAttribute(Qt::WA_DeleteOnClose);
    m_game->setWindowTitle("Тест карты");
    m_game->resize(AppSettings::instance().screenWidth(), AppSettings::instance().screenHeight());

    connect(m_game, &GameWindow::gameFinished, this, &EditorWindow::onTestFinished);

    hide();
    m_game->show();
    m_game->activateWindow();
    m_game->raise();
}

void EditorWindow::onTestFinished(){
    show();
    activateWindow();
    raise();

    m_game->close();
    m_game = nullptr;
}
