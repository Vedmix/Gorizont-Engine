#pragma once
#include <QWidget>
#include <QPushButton>
#include <QPixmap>
#include <QVector>
#include <QPointF>
#include <QRectF>
#include <QPainter>
#include <QMouseEvent>
#include <QLineF>

struct CircleItem {
    QPointF pos;//центр в координатах карты
    double radius;
};

class MapCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit MapCanvas(QWidget* parent = nullptr);

    void setBackground(const QPixmap& pixmap, const QSize& mapSize);
    void addCircle();
    const QVector<CircleItem>& circles() const { return m_circles; }

private slots:
    void onDeleteClicked();

private:
    enum class Drag { None, Move, Resize };

    QRectF mapRect() const;                 //где в виджете лежит карта
    double scale() const;                   //пикселей виджета на единицу карты
    QPointF toMap(const QPointF& widgetPos) const;
    QPointF toWidget(const QPointF& mapPos) const;

    int circleAt(const QPointF& mapPos) const;
    bool onEdge(const CircleItem& circle, const QPointF& widgetPos) const;
    double maxRadiusAt(const QPointF& center) const;
    void clampCircle(CircleItem& circle) const;
    void updateDeleteButton();              //ставит кнопку над выбранным кругом

    QPixmap m_background;
    QSize m_mapSize;

    QVector<CircleItem> m_circles;//вектор кругов
    int m_selected = -1;

    Drag m_drag = Drag::None;
    QPointF m_dragOffset;

    QPushButton* m_deleteButton;

    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
