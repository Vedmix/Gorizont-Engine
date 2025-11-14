#ifndef SFML_WIDGET_H
#define SFML_WIDGET_H

#include <QWidget>
#include <SFML/Graphics.hpp>
#include <QTimer>
#include <QPixmap>

class SFMLWidget : public QWidget {
    Q_OBJECT

public:
    SFMLWidget(QWidget *parent = nullptr);
    ~SFMLWidget();

    void setCircleColor(int r, int g, int b);
    void setCircleRadius(float radius);
    void initializeSFML();

public slots:
    void updateAnimation();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    void renderFrame();

    sf::RenderTexture renderTexture;
    sf::CircleShape circle;
    float circleRadius;
    sf::Color circleColor;
    float animationAngle;
    bool initialized;
    QTimer *renderTimer;
    QPixmap pixmap;
};

#endif // SFML_WIDGET_H