#include "../headers/MapCanvas.hpp"

namespace {
constexpr double kDefaultRadius = 50;
constexpr double kMinRadius = 10;
constexpr double kMaxRadius = 3000;
constexpr double kButtonSize = 24;
constexpr double kButtonGap = 10;     // больше kGrabMargin, чтобы кнопка не перекрывала границу
constexpr double kGrabMargin = 8;     // насколько близко к границе (в пикселях) начинается растягивание
}

MapCanvas::MapCanvas(QWidget* parent) : QWidget(parent)
{
    setMinimumSize(1, 1);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    setMouseTracking(true);
}

void MapCanvas::setBackground(const QPixmap& pixmap, const QSize& mapSize, int wallThickness)
{
    m_background = pixmap;
    m_mapSize = mapSize;
    m_wall = wallThickness;
    update();
}

void MapCanvas::addCircle()
{
    CircleItem c{QPointF(m_mapSize.width() / 2.0, m_mapSize.height() / 2.0), kDefaultRadius};
    m_circles.push_back(c);
    m_selected = m_circles.size() - 1;
    update();
}

// ---------- координаты ----------
QRectF MapCanvas::mapRect() const
{
    if (m_mapSize.isEmpty()) return QRectF();
    const QSizeF s = QSizeF(m_mapSize).scaled(QSizeF(size()), Qt::KeepAspectRatio);
    return QRectF(QPointF((width() - s.width()) / 2, (height() - s.height()) / 2), s);
}

double MapCanvas::scale() const
{
    return m_mapSize.isEmpty() ? 1.0 : mapRect().width() / m_mapSize.width();
}

QPointF MapCanvas::toMap(const QPointF& w) const
{
    return (w - mapRect().topLeft()) / scale();
}

QPointF MapCanvas::toWidget(const QPointF& m) const
{
    return mapRect().topLeft() + m * scale();
}

// ---------- поиск элементов под курсором ----------
int MapCanvas::circleAt(const QPointF& mapPos) const
{
    for (int i = m_circles.size() - 1; i >= 0; --i) {   // верхние круги проверяем первыми
        const QPointF d = mapPos - m_circles[i].pos;
        if (d.x() * d.x() + d.y() * d.y() <= m_circles[i].radius * m_circles[i].radius)
            return i;
    }
    return -1;
}

QRectF MapCanvas::deleteButtonRect() const
{
    const CircleItem& c = m_circles[m_selected];
    const QPointF center = toWidget(c.pos);
    const double x = center.x() - kButtonSize / 2;
    const double y = qMax(0.0, center.y() - c.radius * scale() - kButtonSize - kButtonGap);
    return QRectF(x, y, kButtonSize, kButtonSize);
}

bool MapCanvas::onEdge(const CircleItem& c, const QPointF& widgetPos) const
{
    const QPointF d = widgetPos - toWidget(c.pos);
    const double dist = std::hypot(d.x(), d.y());
    return std::abs(dist - c.radius * scale()) <= kGrabMargin;
}

double MapCanvas::maxRadiusAt(const QPointF& p) const
{
    return qMin(qMin(kMaxRadius, p.x() - m_wall), qMin(qMin(m_mapSize.width()  - m_wall - p.x(), p.y() - m_wall), m_mapSize.height() - m_wall - p.y()));
}

void MapCanvas::clampCircle(CircleItem& c) const
{
    c.pos.setX(qBound(m_wall + c.radius, c.pos.x(), m_mapSize.width()  - m_wall - c.radius));
    c.pos.setY(qBound(m_wall + c.radius, c.pos.y(), m_mapSize.height() - m_wall - c.radius));
}

// ---------- рисование ----------
void MapCanvas::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    if (!m_background.isNull()) {
        p.drawPixmap(mapRect(), m_background, QRectF(m_background.rect()));
    }

    for (int i = 0; i < m_circles.size(); ++i) {
        const QPointF center = toWidget(m_circles[i].pos);
        const double r = m_circles[i].radius * scale();
        p.setBrush(QColor(200, 200, 200));
        p.setPen(i == m_selected ? QPen(QColor(80, 160, 255), 2) : QPen(Qt::NoPen));
        p.drawEllipse(center, r, r);
    }

    if (m_selected >= 0) {
        // Точки на границе подсказывают, что круг можно растягивать
        const QPointF center = toWidget(m_circles[m_selected].pos);
        const double r = m_circles[m_selected].radius * scale();
        p.setPen(QPen(QColor(80, 160, 255), 2));
        p.setBrush(Qt::white);
        for (const QPointF& d : {QPointF(r, 0), QPointF(-r, 0), QPointF(0, r), QPointF(0, -r)}) {
            p.drawEllipse(center + d, 4, 4);
        }

        // Кнопка удаления
        const QRectF r2 = deleteButtonRect();
        QFont font = p.font();
        font.setBold(true);
        p.setFont(font);
        p.setPen(QPen(Qt::white, 1));
        p.setBrush(QColor(200, 60, 60));
        p.drawRoundedRect(r2, 4, 4);
        p.drawText(r2, Qt::AlignCenter, "×");
    }
}

void MapCanvas::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    const QPointF w = event->localPos();

    // 1. Кнопка удаления у выбранного круга
    if (m_selected >= 0 && deleteButtonRect().contains(w)) {
        m_circles.remove(m_selected);
        m_selected = -1;
        update();
        return;
    }

    // 2. Граница выбранного круга - начинаем растягивать
    if (m_selected >= 0 && onEdge(m_circles[m_selected], w)) {
        m_drag = Drag::Resize;
        return;
    }

    // 3. Круг - выбираем и начинаем двигать; пустое место - снимаем выбор
    const QPointF m = toMap(w);
    m_selected = circleAt(m);
    if (m_selected >= 0) {
        m_drag = Drag::Move;
        m_dragOffset = m_circles[m_selected].pos - m;
    }
    update();
}

void MapCanvas::mouseMoveEvent(QMouseEvent* event)
{
    const QPointF w = event->localPos();

    if (m_drag == Drag::Move) {
        m_circles[m_selected].pos = toMap(w) + m_dragOffset;
        clampCircle(m_circles[m_selected]);
        update();
    }
    else if (m_drag == Drag::Resize) {
        CircleItem& c = m_circles[m_selected];
        const QPointF d = toMap(w) - c.pos;
        const double dist = std::hypot(d.x(), d.y());
        c.radius = qBound(kMinRadius, dist, maxRadiusAt(c.pos));
        update();
    }
    else {
        // Просто наведение - подсказываем курсором, что можно сделать
        if (m_selected >= 0 && onEdge(m_circles[m_selected], w))
            setCursor(Qt::SizeFDiagCursor);
        else if (circleAt(toMap(w)) >= 0)
            setCursor(Qt::SizeAllCursor);
        else
            setCursor(Qt::ArrowCursor);
    }
}

void MapCanvas::mouseReleaseEvent(QMouseEvent*)
{
    m_drag = Drag::None;
}
