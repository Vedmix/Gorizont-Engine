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
#include <set>
#include <memory>

#include "Circle.hpp"
#include "Object2D.hpp"
#include "Point2D.hpp"
#include "Wall.hpp"

class MapCanvas : public QWidget
{
    Q_OBJECT

public:
    explicit MapCanvas(QWidget* parent = nullptr);

    void setBackground(const QPixmap& pixmap, const QSize& mapSize);
    void addCircle();
    void addWall();
const std::set<std::shared_ptr<Object2D>>& objects() const { return m_objects; }
private slots:
    void onDeleteClicked();

private:
    enum class Drag { None, Move, Resize };

    QRectF mapRect() const;                 //где в виджете лежит карта
    double scale() const;                   //пикселей виджета на единицу карты
    QPointF toMap(const QPointF& widgetPos) const;
    QPointF toWidget(const QPointF& mapPos) const;

    std::shared_ptr<Object2D> objectAt(const QPointF& mapPos) const;
    void updateDeleteButton();              //ставит кнопку над выбранным кругом

    QPixmap m_background;
    QSize m_mapSize;

    std::set<std::shared_ptr<Object2D>> m_objects;
    std::shared_ptr<Object2D> m_selected;     // пусто = ничего не выбрано
    int m_handle = -1;

    Drag m_drag = Drag::None;
    QPointF m_dragOffset;

    QPushButton* m_deleteButton;

    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
};
