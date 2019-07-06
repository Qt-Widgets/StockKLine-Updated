#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include "bottomtimegrid.h"

BottomTimeGrid::BottomTimeGrid(MarketDataSplitter* parent, DataFile* dataFile)
    : DataWidget(parent, dataFile, false)
{
    this->setFixedHeight(tipsHeight);
}

void BottomTimeGrid::paintEvent(QPaintEvent* event)
{
    if (!bCross) {
        return;
    } else if (mousePoint.x() < getMarginLeft() || mousePoint.x() > getGridWidth() + getMarginLeft()) {
        return;
    }

    QPainter painter(this);
    QPen     pen;
    QBrush brush(QColor(64,0,128));
    painter.setBrush(brush);
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    QRect rect(mousePoint.x(), 0, tipsWidth, tipsHeight);
    painter.drawRect(rect);

    int currentDayAtMouse = ( mousePoint.x() - getMarginLeft() ) * totalDay / getGridWidth() + beginDay;
    if( currentDayAtMouse >= endDay) {
        currentDayAtMouse = endDay;
    } else if (currentDayAtMouse <= beginDay) {
        currentDayAtMouse = beginDay;
    }

    QRect rectText(mousePoint.x(), 0, tipsWidth, tipsHeight);
    painter.drawText(rectText, mDataFile->kline[currentDayAtMouse].time);
}
