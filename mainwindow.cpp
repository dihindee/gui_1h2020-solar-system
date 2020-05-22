#include "mainwindow.h"

#include <QHBoxLayout>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    listModel = new BodyListModel(this);
    listView = new QListView(this);
    listView->setModel(listModel);
    listView->setSelectionMode(QAbstractItemView::SingleSelection);
    central->setLayout(new QHBoxLayout);
    QVBoxLayout* leftPanel = new QVBoxLayout;
    leftPanel->addWidget(listView);
    btnDel = new QPushButton(QString::fromStdString("Delete selected"),this);
    leftPanel->addWidget(btnDel);
    editWidget = new BodyEditWidget(this);
    editWidget->setComboBoxModel(listModel);
    leftPanel->addWidget(editWidget);
    central->layout()->addItem(leftPanel);
    graphicsView = new BodyGraphicsView(this);
    central->layout()->addWidget(graphicsView);
    CelestialBody *body = new CelestialBody();
    listModel->addBody(body);
    graphicsView->scene()->addItem(body);
    setCentralWidget(central);

    editWidget->setSelection(body);
    graphicsView->setSelection(body);
    listView->setCurrentIndex(listModel->index(0));

    connect(editWidget, &BodyEditWidget::fieldEdited, this, [this](){
        this->graphicsView->scene()->update();
        emit listModel->layoutChanged();
    });
    connect(editWidget, SIGNAL(fieldEdited()), listView, SLOT(repaint()));
    connect(graphicsView, &BodyGraphicsView::selectionChanged, editWidget, &BodyEditWidget::setSelection);
    connect(graphicsView, &BodyGraphicsView::selectionChanged, this, [this](CelestialBody *body){
        QModelIndex index = listModel->index(listModel->indexOf(body));
        listView->setCurrentIndex(index);
    });
    connect(graphicsView, &BodyGraphicsView::selectionUpdated, editWidget, &BodyEditWidget::stepUpdate);
    connect(graphicsView, &BodyGraphicsView::bodyCreated, listModel, &BodyListModel::addBody);
    connect(btnDel, &QPushButton::clicked, this, [this](){
        CelestialBody* b = listModel->removeBody(graphicsView->getSelection());
        for(int i = 0; i < listModel->rowCount(QModelIndex()); i ++){
            CelestialBody *body = listModel->getBodyAt(i);
            if(body->orbitTarget == b)
                body->orbitTarget = body;

        }
        graphicsView->setSelection(nullptr);
        if(b != nullptr)
            graphicsView->scene()->removeItem(b);
        editWidget->setSelection(nullptr);
        delete b;
    });
    connect(listView,&QListView::pressed, this, [this](const QModelIndex &m){
        CelestialBody *body = listModel->getBodyAt(m.row());
        graphicsView->setSelection(body);
        graphicsView->scene()->update();
        editWidget->setSelection(body);
    });
}

MainWindow::~MainWindow(){

}


