#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListView>
#include <QMainWindow>
#include <QPushButton>
#include "bodylistmodel.h"
#include "bodygraphicsview.h"
#include "bodyeditwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
    BodyListModel *listModel;
    QListView* listView;
    QPushButton* btnDel;
    BodyGraphicsView* graphicsView;
    BodyEditWidget* editWidget;
    int selectedItem;
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
};
#endif // MAINWINDOW_H
