#ifndef BODYGRAPHICSVIEW_H
#define BODYGRAPHICSVIEW_H
#include <QGraphicsView>
#include "celestialbody.h"
class BodyGraphicsView : public QGraphicsView
{
    Q_OBJECT
    CelestialBody *selection;
    int timerId = 0;
    bool isPaused = true;
public:
    explicit BodyGraphicsView(QWidget *parent);
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void drawForeground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void timerEvent(QTimerEvent *event) override;
//    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    CelestialBody* getSelection();
public slots:
    void setSelection(CelestialBody *selection);
signals:
    void selectionChanged(CelestialBody* newSelection);
    void bodyCreated(CelestialBody* newBody);
    void selectionUpdated();
};

#endif // BODYGRAPHICSVIEW_H
