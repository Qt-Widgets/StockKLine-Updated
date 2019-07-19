#include <iostream>
#include "datawidget.h"

DataWidget::DataWidget(MarketDataSplitter* parent, DataFile* dataFile, bool needGrid)
    : AutoGrid(parent, needGrid)
{
    this->mDataFile = dataFile;

    connect(parent, &MarketDataSplitter::childMouseMoved, this, &DataWidget::mouseMoveEventFromParent);
    connect(parent, &MarketDataSplitter::childMousePressed, this, &DataWidget::mousePressEventFromParent);
    connect(parent, &MarketDataSplitter::childMouseReleased, this, &DataWidget::mouseReleaseEventFromParent);
    connect(parent, &MarketDataSplitter::childKeyPressed, this, &DataWidget::keyPressEventFromParent);

    endDay = mDataFile->kline.size() - 1;
    totalDay = 200;
    beginDay  = endDay - totalDay;
    currentDay = beginDay + totalDay /2;
    if( beginDay < 0)
    {
        beginDay = 0;
        totalDay = mDataFile->kline.size();
    }
}

void DataWidget::keyPressEventFromParent(QKeyEvent* event)
{
    currentDay = (double)( mousePoint.x() - getMarginLeft() ) / (getGridWidth()) * totalDay + beginDay;

    isKeyDown = true;
    switch(event->key())
    {
    case Qt::Key_Left:
    {
        double xstep = std::max(getGridWidth() / totalDay, 1.0);

        if( mousePoint.x() - xstep < getMarginLeft())
        {
            if( beginDay - 1 < 0)
                return;
            endDay -= 1;
            beginDay -= 1;
        }
        else
            mousePoint.setX(mousePoint.x() - xstep);

        update();
        break;
    }

    case Qt::Key_Right:
    {
        double xstep = std::max(getGridWidth() / totalDay, 1.0);

        if( mousePoint.x() + xstep > getWidgetWidth() - getMarginRight())
        {
            if( endDay + 1 > mDataFile->kline.size() - 1)
                return;
            endDay += 1;
            beginDay += 1;
        }
        else
            mousePoint.setX(mousePoint.x() + xstep);

        update();
        break;
    }

    case Qt::Key_Up:
    {
        totalDay = totalDay /2;

        //最少显示10个
        if( totalDay < 10)
        {
            totalDay *= 2;
            return;
        }

        endDay = currentDay + (endDay - currentDay) / 2;
        beginDay = currentDay - (totalDay - (endDay - currentDay));

        if( endDay > mDataFile->kline.size() -10)
        {
            endDay = mDataFile->kline.size() -10;
            beginDay = endDay - totalDay;
        }

        if(beginDay < 0 )
        {
            beginDay = 0;
            endDay = beginDay + totalDay;
        }

        update();


        break;
    }

    case Qt::Key_Down:
    {
        int currentTotalDay = totalDay;

        if(totalDay == mDataFile->kline.size() - 1)
            return;

        totalDay = totalDay * 2;
        if( totalDay > mDataFile->kline.size() - 1)
        {
            totalDay = mDataFile->kline.size() - 1;
        }


        endDay = currentDay + (int)((float)(endDay - currentDay) / currentTotalDay * totalDay);
        if( endDay > mDataFile->kline.size() - 1)
        {
            endDay = mDataFile->kline.size() - 1;
        }

        beginDay = currentDay - (totalDay - (endDay - currentDay));
        if( beginDay < 0)
            beginDay = 0;

        totalDay = endDay - beginDay;
        mousePoint.setX((double)(currentDay - beginDay) / totalDay * getGridWidth() + getMarginLeft());

        update();
    }
    default:
        break;
    }
}

void DataWidget::mouseMoveEventFromParent(QMouseEvent* event)
{
    if (!isMouseReleased) {
        bCross = false;
        moveDataWindow(event);
    }

    isUnderMouse = this->underMouse();
    mousePoint = event->pos();
    isKeyDown = false;
    update();
}

void DataWidget::mousePressEventFromParent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        isMouseReleased = false;
        mousePressedPoint = event->pos();
        //bCross = !bCross;
        //update();
    }
}

void DataWidget::mouseReleaseEventFromParent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton) {
        isMouseReleased = true;

        //no mouse movement
        if (event->pos().x() == mousePressedPoint.x() && event->y() == mousePressedPoint.y()) {
            bCross = !bCross;
        }

        update();
    }
}

void DataWidget::resizeEvent(QResizeEvent* event)
{
    AutoGrid::resizeEvent(event);
    bCross = false;
}

void DataWidget::moveDataWindow(QMouseEvent* event)
{
    int xDelta = event->x() - mousePoint.x();
    int interval = (endDay - beginDay) * std::fabs(xDelta) / (getWidgetWidth() - getMarginLeft() - getMarginRight());
    if (xDelta > 0) {
        if( beginDay - interval < 0) {
            endDay -= beginDay;
            beginDay = 0;
        } else {
            endDay -= interval;
            beginDay -= interval;
        }
    } else if (xDelta < 0) {
        if( endDay + interval > mDataFile->kline.size() - 1) {
            int remaining = mDataFile->kline.size() - 1 - endDay;
            beginDay += remaining;
            endDay = mDataFile->kline.size() - 1;
        } else {
            endDay += interval;
            beginDay += interval;
        }
    }
}
