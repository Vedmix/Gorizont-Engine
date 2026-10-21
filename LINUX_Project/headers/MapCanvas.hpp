#pragma once
#include <QWidget>
#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QRectF>
#include <cmath>
#include <QPainter>
#include <QMouseEvent>


struct CircleItem {
    QPointF pos;     // центр в координатах карты
    double radius;
};

class MapCanvas : public QWidget
{
    Q_OBJECT
public:
    explicit MapCanvas(QWidget* parent = nullptr);

    void setBackground(const QPixmap& pixmap, const QSize& mapSize, int wallThickness);
    void addCircle();
    const QVector<CircleItem>& circles() const { return m_circles; }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

private:
    enum class Drag { None, Move, Resize };

    QPixmap m_background;
    QSize m_mapSize;
    int m_wall = 30;
    QVector<CircleItem> m_circles;
    int m_selected = -1;
    Drag m_drag = Drag::None;
    QPointF m_dragOffset;

    QRectF mapRect() const;
    double scale() const;
    QPointF toMap(const QPointF& w) const;
    QPointF toWidget(const QPointF& m) const;
    int circleAt(const QPointF& mapPos) const;
    bool onEdge(const CircleItem& c, const QPointF& widgetPos) const;  // курсор у границы круга?
    double maxRadiusAt(const QPointF& center) const;                   // радиус не должен вылезать за стены
    QRectF deleteButtonRect() const;
    void clampCircle(CircleItem& c) const;


};
