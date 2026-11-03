#include "../headers/MapCanvas.hpp"

MapCanvas::MapCanvas(QWidget* parent) : QWidget(parent)
{
    setMinimumSize(1, 1);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setMouseTracking(true);

    m_deleteButton = new QPushButton("х", this);
    m_deleteButton->setFixedSize(35, 35);
    m_deleteButton->hide();
    connect(m_deleteButton, &QPushButton::clicked, this, &MapCanvas::onDeleteClicked);
}

void MapCanvas::setBackground(const QPixmap& pixmap, const QSize& mapSize)
{
    m_background = pixmap;
    m_mapSize = mapSize;
    update();
}

void MapCanvas::addCircle()
{
    auto circle = std::make_shared<Circle>(Point2D(m_mapSize.width() / 2.0, m_mapSize.height() / 2.0), 30, 0xC8C8C8FF);
    m_circles.push_back(circle);

    m_selected = m_circles.size() - 1;

    updateDeleteButton();
    update();
}

void MapCanvas::onDeleteClicked()
{
    if (m_selected < 0) {
        return;
    }

    m_circles.remove(m_selected);
    m_selected = -1;

    updateDeleteButton();
    update();
}

QRectF MapCanvas::mapRect() const
{
    if (m_mapSize.isEmpty()) {
        return QRectF();
    }

    QSizeF scaled = QSizeF(m_mapSize).scaled(QSizeF(size()), Qt::KeepAspectRatio);
    double x = (width() - scaled.width()) / 2;
    double y = (height() - scaled.height()) / 2;
    return QRectF(QPointF(x, y), scaled);
}

double MapCanvas::scale() const
{
    if (m_mapSize.isEmpty()) {
        return 1.0;
    }
    return mapRect().width() / m_mapSize.width();
}

QPointF MapCanvas::toMap(const QPointF& widgetPos) const
{
    return (widgetPos - mapRect().topLeft()) / scale();
}

QPointF MapCanvas::toWidget(const QPointF& mapPos) const
{
    return mapRect().topLeft() + mapPos * scale();
}

int MapCanvas::circleAt(const QPointF& mapPos) const
{
    for (int i = m_circles.size() - 1; i >= 0; --i) {
        if (m_circles[i]->isCrossing(Point2D(mapPos.x(), mapPos.y()))) {
            return i;
        }
    }
    return -1;
}

void MapCanvas::updateDeleteButton()
{
    if (m_selected < 0) {
        m_deleteButton->hide();
        return;
    }

    QRectF rect = m_circles[m_selected]->boundingRect();
    QPointF top = toWidget(QPointF(rect.center().x(), rect.top()));

    int x = int(top.x()) - m_deleteButton->width() / 2;
    int y = int(top.y()) - m_deleteButton->height() - 10;
    if (y < 0) {
        y = 0;
    }

    m_deleteButton->move(x, y);
    m_deleteButton->show();
    m_deleteButton->raise();
}

void MapCanvas::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    if (!m_background.isNull()) {
        painter.drawPixmap(mapRect(), m_background, QRectF(m_background.rect()));
    }

    painter.save();
    painter.translate(mapRect().topLeft());
    painter.scale(scale(), scale());

    for (const auto& circle : m_circles) {
        circle->drawQt(painter);
    }
    if (m_selected >= 0) {
        m_circles[m_selected]->drawSelection(painter);
    }
    painter.restore();
}

void MapCanvas::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateDeleteButton();
}

void MapCanvas::mousePressEvent(QMouseEvent* event)
{
    QPointF widgetPos = event->pos();
    QPointF mapPos = toMap(widgetPos);

    if (event->button() != Qt::LeftButton) {
        return;
    }

    //Отработка границы
    if (m_selected >= 0) {
        double tolerance = 8 / scale();   // 8 пикселей экрана в единицах карты
        if (m_circles[m_selected]->hitHandle(Point2D(mapPos.x(), mapPos.y()), tolerance) >= 0) {
            m_drag = Drag::Resize;
            return;
        }
    }

    m_selected = circleAt(mapPos);

    if (m_selected >= 0) {
        Point2D pos = m_circles[m_selected]->getPos();
        m_dragOffset = QPointF(pos.getX(), pos.getY()) - mapPos;
        m_drag = Drag::Move;
    }

    updateDeleteButton();
    update();
}

void MapCanvas::mouseMoveEvent(QMouseEvent* event)
{
    QPointF widgetPos = event->pos();
    QPointF mapPos = toMap(widgetPos);

    if (m_drag == Drag::Move) {
        QPointF newPos = mapPos + m_dragOffset;

        //readCirclesXML не читает минус
        newPos.setX(qBound(0.0, newPos.x(), double(m_mapSize.width())));
        newPos.setY(qBound(0.0, newPos.y(), double(m_mapSize.height())));

        m_circles[m_selected]->setPos(Point2D(newPos.x(), newPos.y()));

        updateDeleteButton();
        update();
        return;
    }

    if (m_drag == Drag::Resize) {

        m_circles[m_selected]->dragHandle(0, Point2D(mapPos.x(), mapPos.y()));

        updateDeleteButton();
        update();
        return;
    }

    bool onEdge = false;
    if (m_selected >= 0) {
        double tolerance = 8 / scale();
        onEdge = m_circles[m_selected]->hitHandle(Point2D(mapPos.x(), mapPos.y()), tolerance) >= 0;
    }

    if (onEdge) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (circleAt(mapPos) >= 0) {
        setCursor(Qt::SizeAllCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void MapCanvas::mouseReleaseEvent(QMouseEvent*)
{
    m_drag = Drag::None;
}
