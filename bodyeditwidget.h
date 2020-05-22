#ifndef BODYEDITWIDGET_H
#define BODYEDITWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QColorDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include "celestialbody.h"
class BodyEditWidget : public QWidget
{
    Q_OBJECT
    CelestialBody *selectedBody;

    QLineEdit *nameEdit;
    QLineEdit *radiusEdit;
    QLineEdit *surfGravityEdit;
    QLineEdit *posXEdit,*posYEdit;
    QLineEdit *velXEdit,*velYEdit;
    QPushButton *colorChooseButton;
    QColorDialog *colorChooser;

    QCheckBox *drawOrbitBox;
    QLineEdit *orbitStepEdit;
    QComboBox *targetChooser;
public:
    explicit BodyEditWidget(QWidget *parent = nullptr);
    void setComboBoxModel(QAbstractItemModel *model);
signals:
    void fieldEdited();
public slots:
    void setSelection(CelestialBody *body);
    void updateFields();
    void stepUpdate();
    void updateBody();
    void chooseColor();
};

#endif // BODYEDITWIDGET_H
