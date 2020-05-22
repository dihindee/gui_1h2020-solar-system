#include "bodylistmodel.h"

int BodyListModel::rowCount(const QModelIndex &parent) const
{
    return m_data.size();
}

QVariant BodyListModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole){
        return m_data.at(index.row())->bodyName;
    }
    else
        return QVariant();
}

BodyListModel::BodyListModel(QObject* parent) : QAbstractListModel(parent)
{
}

void BodyListModel::addBody(CelestialBody *newBody)
{
    m_data.append(newBody);
    emit layoutChanged();
}

CelestialBody* BodyListModel::removeBody(int index)
{
    CelestialBody *body = m_data.at(index);
    m_data.removeAt(index);
    emit layoutChanged();
    return body;
}

CelestialBody *BodyListModel::removeBody(CelestialBody *ptr)
{
    for(int i = 0; i < m_data.size(); i++){
        if(m_data[i] == ptr){
            removeBody(i);
            return ptr;
        }
    }
    return nullptr;
}

int BodyListModel::indexOf(CelestialBody *body)
{
    return m_data.indexOf(body);
}

CelestialBody *BodyListModel::getBodyAt(int index)
{
    return m_data.at(index);
}
