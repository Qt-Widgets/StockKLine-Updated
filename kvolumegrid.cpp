#include "kvolumegrid.h"
#include "marketdatasplitter.h"

#include <QPainter>
#include <QPen>
#include <QString>
#include <QMouseEvent>
#include <iostream>


kVolumeGrid::kVolumeGrid(MarketDataSplitter* parent, DataFile* dataFile)
    : DataWidget( parent, dataFile)
{
    //开启鼠标追踪
    setMouseTracking(true);

    setAtomGridHeightMin(40);
    initial();

    connect(parent, &MarketDataSplitter::childMouseMoved, this, &kVolumeGrid::mouseMoveEventFromParent);
    connect(parent, &MarketDataSplitter::childMousePressed, this, &kVolumeGrid::mousePressEventFromParent);
    connect(parent, &MarketDataSplitter::childKeyPressed, this, &kVolumeGrid::keyPressEventFromParent);
}

void kVolumeGrid::mouseMoveEvent(QMouseEvent* event)
{
    static_cast<MarketDataSplitter*>(parent())->childMouseMoveEvent(event);
}

void kVolumeGrid::keyPressEvent(QKeyEvent* event)
{
    static_cast<MarketDataSplitter*>(parent())->childKeyPressEvent(event);
}

void kVolumeGrid::mousePressEvent(QMouseEvent* event)
{
    static_cast<MarketDataSplitter*>(parent())->childMousePressEvent(event);
}

void kVolumeGrid::mousePressEventFromParent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        bCross = !bCross;
        update();
    }
}

void kVolumeGrid::resizeEvent(QResizeEvent* event)
{

    AutoGrid::resizeEvent(event);
    bCross = false;

}

void kVolumeGrid::paintEvent(QPaintEvent *event)
{

    AutoGrid::paintEvent(event);
    //画y轴坐标轴
    drawYtick();
    drawVolume();

    //画十字线
    if( !isKeyDown && bCross)
    {
        drawCross2();
    }

    if(isKeyDown && bCross)
    {
        drawCross();
    }

    drawAverageLine(5);
    drawAverageLine(10);

}

void kVolumeGrid::initial()
{
    //初始化一些成员变量值

    endDay = mDataFile->kline.size() - 1;
    totalDay = 200;
    beginDay  = endDay - totalDay;
    currentDay = beginDay + totalDay /2;
    maxVolume = 0;

}

//获得画图所需的各项指标
void kVolumeGrid::getIndicator()
{
    maxVolume = 0;
    for(int i=beginDay;i<endDay;++i)
    {
        QString strVolume = mDataFile->kline[i].totalVolume;
        strVolume = strVolume.mid(1,strVolume.length());
        strVolume = strVolume.mid(0,strVolume.length()-1);
        strVolume.replace(QString(","),QString(""));
        if( strVolume.toInt() > maxVolume)
            maxVolume = strVolume.toInt();
    }
    maxVolume = maxVolume / 100;
}

void kVolumeGrid::keyPressEventFromParent(QKeyEvent *event)
{
    currentDay = (double)( mousePoint.x() - getMarginLeft() ) / (getGridWidth()) * totalDay + beginDay;

    isKeyDown = true;
    switch(event->key())
    {
    case Qt::Key_Left:
    {
        double xstep = getGridWidth() / totalDay ;

        if( mousePoint.x() - xstep < getMarginLeft())
        {
            if( beginDay -1 < 0)
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
        double xstep = getGridWidth() / totalDay ;

        if( mousePoint.x() + xstep > getWidgetWidth() - getMarginRight())
        {
            if( endDay +1 > mDataFile->kline.size() -1)
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

        if(totalDay == mDataFile->kline.size() -1 )
            return;

        totalDay = totalDay * 2;
        if( totalDay > mDataFile->kline.size() -1)
        {
            totalDay = mDataFile->kline.size() -1;
        }


        endDay = currentDay + (int)((float)(endDay - currentDay) / currentTotalDay * totalDay);
        if( endDay > mDataFile->kline.size() -1)
        {
            endDay = mDataFile->kline.size() -1;
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

void kVolumeGrid::mouseMoveEventFromParent(QMouseEvent *event)
{
    isUnderMouse = this->underMouse();
    mousePoint = event->pos();
    isKeyDown = false;
    update();
}

void kVolumeGrid::drawYtick()
{

    getIndicator();

    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    double ystep = maxVolume / getHGridNum() ;
    QString str;
    for( int i=0;i<=getHGridNum();++i)
    {
        str.sprintf("%d", (int)(i*ystep) );
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  getWidgetHeight() - getMarginBottom() - i*getAtomGridHeight()),
                          str);
    }
}
void kVolumeGrid::drawVolume()
{
    QPainter painter(this);
    QPen     pen;


    double xstep = getGridWidth() / totalDay;
    double yscale = getGridHeight() / maxVolume;

    for( int i= beginDay;i<endDay;++i)
    {
        if( mDataFile->kline[i].openingPrice > mDataFile->kline[i].closeingPrice )
            pen.setColor(QColor(85,252,252));
        else
            pen.setColor(Qt::red);


        lineWidth = getGridWidth() / totalDay;

        //为了各个k线之间不贴在一起，设置一个间隔
        lineWidth = lineWidth - 0.2*lineWidth;

        //最小线宽为3
        if( lineWidth < 3)
            lineWidth = 3;


        //画线连接的两个点
        QPoint p1;
        QPoint p2;

        QPoint p3;
        QPoint p4;



        QString strtemp = mDataFile->kline[i].totalVolume;
        strtemp = strtemp.mid(1,strtemp.length());
        strtemp = strtemp.mid(0,strtemp.length()-1);
        strtemp.replace(QString(","),QString(""));
        int temp = strtemp.toInt()/100;


        //阴线
        if( mDataFile->kline[i].openingPrice > mDataFile->kline[i].closeingPrice )
        {
            pen.setWidth(lineWidth);
            painter.setPen(pen);
            p1.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p1.setY( getWidgetHeight() - (temp ) *yscale - getMarginBottom() + 0.5*lineWidth);
            p2.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p2.setY( getWidgetHeight()  - getMarginBottom() - 0.5*lineWidth);
            painter.drawLine(p1,p2);
        }

        //阳线
        else
        {
            pen.setWidth(1);
            painter.setPen(pen);

            p1.setX( getMarginLeft() + xstep *(i - beginDay) );
            p1.setY( getWidgetHeight() - (temp ) *yscale - getMarginBottom());
            p2.setX(getMarginLeft() + xstep *(i - beginDay) + lineWidth);
            p2.setY( getWidgetHeight() - (temp ) *yscale - getMarginBottom());
            p3.setX( getMarginLeft() + xstep *(i - beginDay) );
            p3.setY( getWidgetHeight()  - getMarginBottom() );
            p4.setX( getMarginLeft() + xstep *(i - beginDay) + lineWidth);
            p4.setY( getWidgetHeight()  - getMarginBottom() );

            painter.drawLine(p1,p2);
            painter.drawLine(p1,p3);
            painter.drawLine(p2,p4);
            painter.drawLine(p3,p4);

        }
    }
}

void kVolumeGrid::drawAverageLine(int day){

    //画线要连接的点
    QVector<QPoint> point;

    //临时点
    QPoint temp;

    //x轴步进
    double xstep = getGridWidth() / totalDay;
    double yscale = getGridHeight() / maxVolume;

    switch(day)
    {
    case 5:
        for( int i= beginDay;i<endDay;++i)
        {
            if( mDataFile->kline[i].volumeAverage5 == 0)
                continue;
            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            temp.setY(getWidgetHeight() - mDataFile->kline[i].volumeAverage5 /100 *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    case 10:
        for( int i= beginDay;i<endDay;++i)
        {
            if( mDataFile->kline[i].volumeAverage10 == 0)
                continue;
            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            temp.setY(getWidgetHeight() - mDataFile->kline[i].volumeAverage10 /100 *yscale - getMarginBottom());
            point.push_back(temp);
        }
        break;
    }

    QPainter painter(this);
    QPen     pen;

    switch(day)
    {
    case 5:
        pen.setColor(Qt::white);
        break;
    case 10:
        pen.setColor(Qt::yellow);
        break;
    }


    painter.setPen(pen);
    QPolygon polykline(point);
    painter.drawPolyline(polykline);

}

void kVolumeGrid::drawCross()
{
    drawCrossVerLine();
    drawCrossHorLine();
}

void kVolumeGrid::drawCrossVerLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    double xstep = getGridWidth() / totalDay ;
    double xPos = getMarginLeft() ;
    while( mousePoint.x() - xPos > xstep )
    {
        xPos += xstep;
    }
    xPos += 0.5*lineWidth;
    QLine horline(xPos,getMarginTop(),xPos,getWidgetHeight() - getMarginBottom());
    painter.drawLine(horline);

}

void kVolumeGrid::drawCrossHorLine()
{
    if(!isUnderMouse)
        return;

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawLine(getMarginLeft(),mousePoint.y(),
                     getWidgetWidth()-getMarginRight(),mousePoint.y());

}

void kVolumeGrid::drawCross2()
{
    drawMouseMoveCrossHorLine();
    drawMouseMoveCrossVerLine();
}

void kVolumeGrid::drawMouseMoveCrossVerLine()
{
    if(mousePoint.x() < getMarginLeft() || mousePoint.x() > getWidgetWidth() - getMarginRight())
        return;

    if (isUnderMouse) {
        if(mousePoint.y() < getMarginTop() || mousePoint.y() > getWidgetHeight() - getMarginBottom())
            return;
    }

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);
    painter.drawLine(mousePoint.x(),getMarginTop(),
                     mousePoint.x(),getWidgetHeight() - getMarginBottom());

}


void kVolumeGrid::drawMouseMoveCrossHorLine()
{
    if(!isUnderMouse)
        return;

    if(mousePoint.x() < getMarginLeft() || mousePoint.x() > getWidgetWidth() - getMarginRight())
        return;

    if(mousePoint.y() < getMarginTop() || mousePoint.y() > getWidgetHeight() - getMarginBottom())
        return;

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawLine(getMarginLeft(),mousePoint.y(),
                     getWidgetWidth()-getMarginRight(),mousePoint.y());

}

