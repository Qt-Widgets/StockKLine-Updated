#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include "bottomtimegrid.h"

BottomTimeGrid::BottomTimeGrid(MarketDataSplitter* parent, DataFile* dataFile)
    : DataWidget(parent, dataFile, false)
{
    this->setFixedHeight(20);

    connect(parent, &MarketDataSplitter::childMouseMoved, this, &BottomTimeGrid::mouseMoveEventFromParent);
    connect(parent, &MarketDataSplitter::childMousePressed, this, &BottomTimeGrid::mousePressEventFromParent);
    connect(parent, &MarketDataSplitter::childKeyPressed, this, &BottomTimeGrid::keyPressEventFromParent);
}

void BottomTimeGrid::mouseMoveEventFromParent(QMouseEvent* event)
{

}

void BottomTimeGrid::mousePressEventFromParent(QMouseEvent* event)
{

}

void BottomTimeGrid::keyPressEventFromParent(QKeyEvent* event)
{

}

void BottomTimeGrid::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

}
