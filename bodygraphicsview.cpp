#include "bodygraphicsview.h"
#include <QMenu>
#include <QMouseEvent>
#include <iostream>
#define TIME_STEP 0.01f
#define G_CONST 0.0001f
BodyGraphicsView::BodyGraphicsView(QWidget* parent): QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    scene->setSceneRect(-320,-240,640,480);
    setCacheMode(CacheBackground);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorViewCenter);
    setMinimumSize(660,500);
    centerOn(0,0);
    timerId = startTimer(100/6);
}

void PhysicsStep(QList<CelestialBody*> &bodies){
    for(int i = 0; i < bodies.length(); i++){
        for(int j = i+1; j < bodies.length(); j++){
            QVector2D p1(bodies[i]->position),p2(bodies[j]->position);
            float dist = p1.distanceToPoint(p2);
            float sign = dist<bodies[i]->getRadius() && dist < bodies[j]->getRadius() ? -1 : 1;
            bodies[i]->currentVelocity += (p2-p1).normalized()* bodies[j]->getMass() * sign * G_CONST / (dist*dist);
            bodies[j]->currentVelocity += (p1-p2).normalized()* bodies[i]->getMass() * sign * G_CONST / (dist*dist);
        }
    }
    for(int i = 0; i < bodies.length(); i++){
        bodies[i]->position.setX(bodies[i]->position.x() + bodies[i]->currentVelocity.x()*TIME_STEP);
        bodies[i]->position.setY(bodies[i]->position.y() + bodies[i]->currentVelocity.y()*TIME_STEP);
    }
}

void BodyGraphicsView::drawBackground(QPainter *painter, const QRectF &rect)
{
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->resetTransform();
    QColor black(0,0,0);
    painter->fillRect(0,0,this->width(),this->height(),black);

}

void BodyGraphicsView::drawForeground(QPainter *painter, const QRectF &rect)
{
    if(selection != nullptr){
        painter->setPen(Qt::white);
        painter->drawRect(selection->position.x()-selection->getRadius(),
                          selection->position.y()-selection->getRadius(),
                          2*selection->getRadius(),2*selection->getRadius());
    }
    QList<CelestialBody*> futureBodies;
    QList<QPointF> prevPos;
    int maxIterations = 0;
    auto curBodies = scene()->items();
    for(QGraphicsItem * b : curBodies){
        CelestialBody *fb = new CelestialBody;
        fb->position = ((CelestialBody*)b)->position;
        prevPos.append(fb->position);
        fb->currentVelocity = ((CelestialBody*)b)->currentVelocity;
        fb->setRadius(((CelestialBody*)b)->getRadius());
        fb->setGravity(((CelestialBody*)b)->getGravity());
        fb->color = ((CelestialBody*)b)->color;
        fb->orbitSteps = ((CelestialBody*)b)->orbitSteps;
        fb->drawOrbit = ((CelestialBody*)b)->drawOrbit;
        futureBodies.append(fb);
        if(((CelestialBody*)b)->drawOrbit && ((CelestialBody*)b)->orbitSteps > maxIterations)
            maxIterations = ((CelestialBody*)b)->orbitSteps;
    }
    for(int i = 0; i < futureBodies.length(); i++ ){
        futureBodies[i]->orbitTarget = futureBodies[curBodies.indexOf(((CelestialBody*)curBodies[i])->orbitTarget)];
    }
    for(int i = 0; i < maxIterations; i++){
        PhysicsStep(futureBodies);
        for(int j = 0; j < futureBodies.length(); j++){
            if(futureBodies[j]->drawOrbit && futureBodies[j]->orbitSteps >= i){
                painter->setPen(futureBodies[j]->color);
                QPointF p;
                if(futureBodies[j] == futureBodies[j]->orbitTarget)
                    p = futureBodies[j]->position;
                else
                    p = ((CelestialBody *)(curBodies[j]))->orbitTarget->position
                        - futureBodies[j]->orbitTarget->position + futureBodies[j]->position;
                painter->drawLine(prevPos[j],p);
                prevPos[j] = p;
            }
        }
    }
    if(isPaused){
        painter->fillRect(-width()/2,-height()/2,20,50,QBrush(Qt::white));
        painter->fillRect(-width()/2 + 30,-height()/2,20,50,QBrush(Qt::white));
    }
}

void BodyGraphicsView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    if(event->button() == Qt::LeftButton){
        QPointF p(event->pos().x() - this->width()/2,event->pos().y() - this->height()/2);
        CelestialBody* newSelection = (CelestialBody*)scene()->itemAt(p,transform());
        if(selection != newSelection){
            selection = newSelection;
            if(selection != nullptr)
                selection->position = p;
            scene()->update();
            emit selectionChanged(selection);
        }
    }
}

void BodyGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    scene()->update();
}

void BodyGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(selection != nullptr){
        selection->position.setX(event->pos().x() - this->width()/2);
        selection->position.setY(event->pos().y() - this->height()/2);
        scene()->update();
        emit selectionChanged(selection);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void BodyGraphicsView::contextMenuEvent(QContextMenuEvent *event)
{
    if(scene()->itemAt(event->pos(),transform()) == nullptr){
        QMenu menu(this);
        menu.addAction("Create new body",this,[this,event](){
            CelestialBody *newBody = new CelestialBody;
            newBody->position.setX(event->pos().x() - this->width()/2);
            newBody->position.setY(event->pos().y() - this->height()/2);
            newBody->bodyName = "new Body";
            scene()->addItem(newBody);
            scene()->update();
            emit bodyCreated(newBody);
        });
        menu.exec(event->globalPos());
    }
}

void BodyGraphicsView::timerEvent(QTimerEvent *event)
{
    if(!isPaused)
    {
        QList<QGraphicsItem*> list = scene()->items();
        PhysicsStep(reinterpret_cast<QList<CelestialBody*>&>(list));
        emit selectionUpdated();
        scene()->update();
    }
}

//void BodyGraphicsView::resizeEvent(QResizeEvent *event)
//{
//    std::cout << "Resize: " << event->size().width() << ";" << event->size().height() << std::endl;
//    centerOn(event->size().width()/2,event->size().height()/2);
//    QGraphicsView::resizeEvent(event);
//}

void BodyGraphicsView::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        isPaused = !isPaused;
        scene()->update();
    }
}

CelestialBody *BodyGraphicsView::getSelection()
{
    return selection;
}

void BodyGraphicsView::setSelection(CelestialBody *selection)
{
    this->selection = selection;
}
