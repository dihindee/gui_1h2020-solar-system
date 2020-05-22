#ifndef CELESTIALBODY_H
#define CELESTIALBODY_H
#include <QVector2D>
#include <QGraphicsItem>

class CelestialBody : public QGraphicsItem
{
private:
    float radius;
    float surfaceGravity;
    float mass;
public:

    bool drawOrbit;
    QColor color;

    CelestialBody *orbitTarget;
    int orbitSteps;

    QPointF position;
    QVector2D currentVelocity;
    QString bodyName;

    CelestialBody(QGraphicsItem *parent = nullptr);
    void setRadius(float radius);
    float getRadius();
    void setGravity(float gravity);
    float getGravity();
    float getMass();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};
#endif // CELESTIALBODY_H
