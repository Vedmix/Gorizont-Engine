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
    CircleItem circle;
    circle.pos = QPointF(m_mapSize.width() / 2.0, m_mapSize.height() / 2.0);
    circle.radius = 30;

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

// ---------- поиск и ограничения ----------
int MapCanvas::circleAt(const QPointF& mapPos) const
{
    // Идём с конца списка: верхние круги проверяем первыми
    for (int i = m_circles.size() - 1; i >= 0; --i) {
        double distance = QLineF(mapPos, m_circles[i].pos).length();
        if (distance <= m_circles[i].radius) {
            return i;
        }
    }
    return -1;
}

bool MapCanvas::onEdge(const CircleItem& circle, const QPointF& widgetPos) const
{
    double distance = QLineF(toWidget(circle.pos), widgetPos).length();
    double edge = circle.radius * scale();
    return qAbs(distance - edge) <= 8;
}

void MapCanvas::updateDeleteButton()
{
    if (m_selected < 0) {
        m_deleteButton->hide();
        return;
    }

    const CircleItem& circle = m_circles[m_selected];
    QPointF center = toWidget(circle.pos);
    double top = center.y() - circle.radius * scale();

    int x = int(center.x()) - m_deleteButton->width() / 2;
    int y = int(top) - m_deleteButton->height() - 10; // 10 это отступ кнопки от круга
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

    for (int i = 0; i < m_circles.size(); ++i) {
        QPointF center = toWidget(m_circles[i].pos);
        double radius = m_circles[i].radius * scale();

        painter.setBrush(QColor(200, 200, 200));
        if (i == m_selected) {
            painter.setPen(QPen(QColor(80, 160, 255), 2));
        } else {
            painter.setPen(Qt::NoPen);
        }
        painter.drawEllipse(center, radius, radius);
    }

    // У выбранного круга рисуем 4 точки на границе: за них можно тянуть
    if (m_selected >= 0) {
        QPointF center = toWidget(m_circles[m_selected].pos);
        double radius = m_circles[m_selected].radius * scale();

        QVector<QPointF> handles;
        handles.push_back(center + QPointF(radius, 0));
        handles.push_back(center + QPointF(-radius, 0));
        handles.push_back(center + QPointF(0, radius));
        handles.push_back(center + QPointF(0, -radius));

        painter.setPen(QPen(QColor(80, 160, 255), 2));
        painter.setBrush(Qt::white);
        for (const QPointF& handle : handles) {
            painter.drawEllipse(handle, 4, 4);
        }
    }
}

void MapCanvas::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateDeleteButton();
}

void MapCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) {
        return;
    }

    QPointF widgetPos = event->pos();

    //Отработка границы
    if (m_selected >= 0 && onEdge(m_circles[m_selected], widgetPos)) {
        m_drag = Drag::Resize;
        return;
    }

    //Отработка наведения на центр
    QPointF mapPos = toMap(widgetPos);
    m_selected = circleAt(mapPos);

    if (m_selected >= 0) {
        m_drag = Drag::Move;
        m_dragOffset = m_circles[m_selected].pos - mapPos;
    }

    updateDeleteButton();
    update();
}

void MapCanvas::mouseMoveEvent(QMouseEvent* event)
{
    QPointF widgetPos = event->pos();

    if (m_drag == Drag::Move) {
        CircleItem& circle = m_circles[m_selected];
        circle.pos = toMap(widgetPos) + m_dragOffset;

        updateDeleteButton();
        update();
        return;
    }

    if (m_drag == Drag::Resize) {
        CircleItem& circle = m_circles[m_selected];
        double distance = QLineF(circle.pos, toMap(widgetPos)).length();
        circle.radius = qBound(10.0, distance, 1000.0); //круг растягивается от 10 до 1000

        updateDeleteButton();
        update();
        return;
    }

    // Изменение курсора при навведении на разные области
    if (m_selected >= 0 && onEdge(m_circles[m_selected], widgetPos)) {
        setCursor(Qt::SizeFDiagCursor);
    } else if (circleAt(toMap(widgetPos)) >= 0) {
        setCursor(Qt::SizeAllCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}

void MapCanvas::mouseReleaseEvent(QMouseEvent*)
{
    m_drag = Drag::None;
}
