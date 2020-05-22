#include "celestialbody.h"
#include <QPainter>
#include <iostream>
#define G_CONST 0.0001f
void CelestialBody::setRadius(float radius)
{
    this->radius = radius;
    mass = surfaceGravity*radius*radius / G_CONST;
}

float CelestialBody::getRadius()
{
    return radius;
}

void CelestialBody::setGravity(float gravity)
{
    surfaceGravity = gravity;
    mass = surfaceGravity*radius*radius / G_CONST;
}

float CelestialBody::getGravity()
{
    return surfaceGravity;
}

float CelestialBody::getMass()
{
    return mass;
}

CelestialBody::CelestialBody(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    this->bodyName = "Sun";
    radius = 25;
    setGravity(10);
    orbitSteps = 1000;
    drawOrbit = true;
    color = Qt::yellow;
    orbitTarget = this;
}

void CelestialBody::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(color);
    painter->drawEllipse(position.x()-radius,position.y()-radius,2*radius,2*radius);
}

QRectF CelestialBody::boundingRect() const
{
    return QRectF(position.x()-radius,position.y()-radius, 2*radius, 2*radius);
}

void CelestialBody::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void CelestialBody::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}
