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

    QPushButton* addCircleButton = new QPushButton("Добавить круг", this);
    connect(addCircleButton, &QPushButton::clicked, this, &EditorWindow::onAddCircleClicked);
    toolsLayout->addWidget(addCircleButton);
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
        case 0: connect(button, &QPushButton::clicked, this, &EditorWindow::onTestButtonClicked); break;
        case 1: connect(button, &QPushButton::clicked, this, &EditorWindow::onSaveButtonClicked); break;
        case 2: connect(button, &QPushButton::clicked, this, &EditorWindow::onBackButtonClicked); break;
        }
    }
}

QString EditorWindow::saveTemplateMap(){
    auto& settings = AppSettings::instance();
    const int width  = settings.screenWidth();
    const int height = settings.screenHeight();
    const int thickness = 30;

    const QList<QRect> walls = {
        QRect(0,                 0,                  width,     thickness),
        QRect(0,                 height - thickness, width,     thickness),
        QRect(0,                 0,                  thickness, height - thickness),
        QRect(width - thickness, 0,                  thickness, height - thickness)
    };

    QDir().mkpath("maps");
    const QString path = "maps/template_map.xml";

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)){
        qDebug() << "Cannot write template map:" << path;
        return QString();
    }

    QTextStream out(&file);
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<map>\n     <circles>\n";
    for(const CircleItem& c : m_canvas->circles()){
        out << QString("        <circle x=\"%1\" y=\"%2\" radius=\"%3\" color=\"0xC8C8C8FF\"/>\n")
        .arg(c.pos.x(), 0, 'f', 1).arg(c.pos.y(), 0, 'f', 1).arg(c.radius, 0, 'f', 1);
    }
    out << "    </circles>\n</map>\n";

    return path;
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

void EditorWindow::onAddCircleClicked(){ m_canvas->addCircle(); }

void EditorWindow::onSaveButtonClicked(){ m_templatePath = saveTemplateMap(); }


void EditorWindow::onTestButtonClicked(){
    m_templatePath = saveTemplateMap();   // сохраняем, чтобы игра увидела текущие круги
    if(m_templatePath.isEmpty()) return;

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
