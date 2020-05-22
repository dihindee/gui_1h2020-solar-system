#include "bodyeditwidget.h"
#include "bodylistmodel.h"
#include <QVBoxLayout>
#include <QValidator>
#include <QLabel>
#include <iostream>
BodyEditWidget::BodyEditWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    nameEdit = new QLineEdit(this);
    radiusEdit = new QLineEdit(this);
    surfGravityEdit = new QLineEdit(this);
    posXEdit = new QLineEdit(this);
    posYEdit = new QLineEdit(this);
    velXEdit = new QLineEdit(this);
    velYEdit = new QLineEdit(this);
    colorChooser = new QColorDialog(this);
    colorChooseButton = new QPushButton(this);
    drawOrbitBox = new QCheckBox("draw orbit",this);
    orbitStepEdit = new QLineEdit(this);
    targetChooser = new QComboBox(this);
    QHBoxLayout *line = new QHBoxLayout(this);
    line->addWidget(new QLabel("name ",this));
    line->addWidget(nameEdit);
    layout->addItem(line);
    line = new QHBoxLayout(this);
    line->addWidget(new QLabel("radius ",this));
    line->addWidget(radiusEdit);
    layout->addItem(line);
    line = new QHBoxLayout(this);
    line->addWidget(new QLabel("surface gravity ",this));
    line->addWidget(surfGravityEdit);
    layout->addItem(line);
    line = new QHBoxLayout(this);
    line->addWidget(new QLabel("Position X ",this));
    line->addWidget(posXEdit);
    line->addWidget(new QLabel("Y ",this));
    line->addWidget(posYEdit);
    layout->addItem(line);
    line = new QHBoxLayout(this);
    line->addWidget(new QLabel("Velocity X ",this));
    line->addWidget(velXEdit);
    line->addWidget(new QLabel("Y ",this));
    line->addWidget(velYEdit);
    layout->addItem(line);
    layout->addWidget(colorChooseButton);
    layout->addWidget(drawOrbitBox);
    line = new QHBoxLayout(this);
    line->addWidget(new QLabel("orbit steps ",this));
    line->addWidget(orbitStepEdit);
    layout->addItem(line);
    line = new QHBoxLayout(this);
    line->addWidget(new QLabel("orbit on",this));
    line->addWidget(targetChooser);
    layout->addItem(line);
    setLayout(layout);
    this->layout()->setSpacing(1);
    radiusEdit->setValidator(new QIntValidator(0,150,this));
    surfGravityEdit->setValidator(new QDoubleValidator(0,1000,2,this));
    posXEdit->setValidator(new QIntValidator(-1000,1000,this));
    posYEdit->setValidator(new QIntValidator(-1000,1000,this));
    velXEdit->setValidator(new QIntValidator(-1000,1000,this));
    velYEdit->setValidator(new QIntValidator(-1000,1000,this));
    orbitStepEdit->setValidator(new QIntValidator(0,10000,this));
    setVisible(false);

    connect(nameEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(radiusEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(surfGravityEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(posXEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(posYEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(velXEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(velYEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(drawOrbitBox,&QCheckBox::toggled,this, &BodyEditWidget::updateBody);
    connect(orbitStepEdit,&QLineEdit::textEdited,this, &BodyEditWidget::updateBody);
    connect(colorChooseButton, &QPushButton::clicked, this, &BodyEditWidget::chooseColor);
    connect(targetChooser, &QComboBox::currentTextChanged, this, &BodyEditWidget::updateBody);
}

void BodyEditWidget::setComboBoxModel(QAbstractItemModel *model)
{
    targetChooser->setModel(model);
}

void BodyEditWidget::setSelection(CelestialBody *body)
{
    this->selectedBody = body;
    if(body == nullptr)
        this->setVisible(false);
    else{
        this->setVisible(true);
    }
    updateFields();
}

void BodyEditWidget::updateFields()
{
    if(selectedBody != nullptr){
        nameEdit->setText(selectedBody->bodyName);
        radiusEdit->setText(QString::number(selectedBody->getRadius()));
        surfGravityEdit->setText(QString::number(selectedBody->getGravity()));
        stepUpdate();
        drawOrbitBox->setChecked(selectedBody->drawOrbit);
        orbitStepEdit->setText(QString::number(selectedBody->orbitSteps));

        QPalette pal = colorChooseButton->palette();
        pal.setColor(QPalette::Button, selectedBody->color);
        colorChooseButton->setAutoFillBackground(true);
        colorChooseButton->setPalette(pal);
        colorChooseButton->setFlat(true);
        colorChooseButton->update();
//        targetChooser->setCurrentIndex();
            targetChooser->setCurrentIndex(((BodyListModel*)(targetChooser->model()))->indexOf(selectedBody->orbitTarget));
    }
}

void BodyEditWidget::stepUpdate()
{
    if(selectedBody != nullptr){
        posXEdit->setText(QString::number(selectedBody->position.x()));
        posYEdit->setText(QString::number(selectedBody->position.y()));
        velXEdit->setText(QString::number(selectedBody->currentVelocity.x()));
        velYEdit->setText(QString::number(selectedBody->currentVelocity.y()));
    }
}

void BodyEditWidget::updateBody()
{
    if(selectedBody != nullptr){
        selectedBody->bodyName = nameEdit->text();
        selectedBody->setRadius(radiusEdit->text().toFloat());
        selectedBody->setGravity(surfGravityEdit->text().toFloat());
        selectedBody->position.setX(posXEdit->text().toFloat());
        selectedBody->position.setY(posYEdit->text().toFloat());
        selectedBody->currentVelocity.setX(velXEdit->text().toFloat());
        selectedBody->currentVelocity.setY(velYEdit->text().toFloat());
        selectedBody->drawOrbit = drawOrbitBox->isChecked();
        selectedBody->orbitSteps = orbitStepEdit->text().toInt();

        int index = targetChooser->currentIndex();
        if(index == -1){
            selectedBody->orbitTarget = selectedBody;
        }
        else{
            selectedBody->orbitTarget = ((BodyListModel*)(targetChooser->model()))->getBodyAt(index);
        }
        emit fieldEdited();
    }
}

void BodyEditWidget::chooseColor()
{
    if(selectedBody!= nullptr){
        QColor newColor = colorChooser->getColor(Qt::white,this,"Select color");
        selectedBody->color = newColor;
        emit fieldEdited();
        QPalette pal = colorChooseButton->palette();
        pal.setColor(QPalette::Button, selectedBody->color);
        colorChooseButton->setAutoFillBackground(true);
        colorChooseButton->setPalette(pal);
        colorChooseButton->setFlat(true);
        colorChooseButton->update();
    }
}
