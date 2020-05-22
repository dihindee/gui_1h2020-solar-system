#ifndef BODYLISTMODEL_H
#define BODYLISTMODEL_H
#include <QAbstractListModel>
#include <QList>

#include "celestialbody.h"
class BodyListModel : public QAbstractListModel
{
    Q_OBJECT
    QList<CelestialBody*> m_data;
public:
    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    BodyListModel(QObject *parent);
    void addBody(CelestialBody *newBody);
    CelestialBody* removeBody(int index);
    CelestialBody* removeBody(CelestialBody* ptr);
    int indexOf(CelestialBody *body);
    CelestialBody* getBodyAt(int index);
};

#endif // BODYLISTMODEL_H
