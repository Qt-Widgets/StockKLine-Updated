#include "capitalaveragediffgrid.h"
#include "marketdatasplitter.h"

#include <QPainter>
#include <QPen>
#include <QString>
#include <QMouseEvent>
#include <iostream>
#include <iomanip>
#include <sstream>

CapitalAverageDiffGrid::CapitalAverageDiffGrid(MarketDataSplitter* parent, DataFile* dataFile)
    : DataWidget( parent, dataFile)
{
    //开启鼠标追踪
    setMouseTracking(true);

    initial();
}

void CapitalAverageDiffGrid::mouseMoveEvent(QMouseEvent* event)
{
    static_cast<MarketDataSplitter*>(parent())->childMouseMoveEvent(event);
}

void CapitalAverageDiffGrid::keyPressEvent(QKeyEvent* event)
{
    static_cast<MarketDataSplitter*>(parent())->childKeyPressEvent(event);
}

void CapitalAverageDiffGrid::mousePressEvent(QMouseEvent* event)
{
    static_cast<MarketDataSplitter*>(parent())->childMousePressEvent(event);
}

void CapitalAverageDiffGrid::mouseReleaseEvent(QMouseEvent* event)
{
    static_cast<MarketDataSplitter*>(parent())->childMouseReleaseEvent(event);
}

void CapitalAverageDiffGrid::paintEvent(QPaintEvent *event)
{

    AutoGrid::paintEvent(event);
    //画y轴坐标轴
    getIndicator();
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

    drawTopInfo();
    drawTips();
}

void CapitalAverageDiffGrid::drawTips()
{
    if (!isUnderMouse)
        return;

    if(mousePoint.x() < getMarginLeft() || mousePoint.x() > getWidgetWidth() - getMarginRight())
        return;

    if(mousePoint.y() < getMarginTop() || mousePoint.y() > getWidgetHeight() - getMarginBottom())
        return;

    QPainter painter(this);
    QPen     pen;
    QBrush brush(QColor(64,0,128));
    painter.setBrush(brush);
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    double yval = std::fabs((mousePoint.y() - ypZeroLine)) * maxVolume / (getGridHeight() / 2);
    double yPos = mousePoint.y();

    int iTipsWidth = 60;
    int iTipsHeight = 30;

    QString str;

    QRect rect( getWidgetWidth() - getMarginRight(),
                yPos - iTipsHeight/2,iTipsWidth,iTipsHeight);
    painter.drawRect(rect);


    QRect rectText( getWidgetWidth() - getMarginRight() + iTipsWidth/4,
                yPos - iTipsHeight/4,iTipsWidth,iTipsHeight);
    painter.drawText(rectText, str.sprintf("%.2f", yval));
}

void CapitalAverageDiffGrid::drawYtick()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    QString str;

    double ystep = maxVolume / (getHGridNum() / 2);
    for(int i = 0; i <= getHGridNum(); ++i)
    {
        str.sprintf("%d", (int)(i * ystep) );
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  ypZeroLine - i * getAtomGridHeight()), str);
    }

    for(int i = 1; i <= getHGridNum(); ++i)
    {
        str.sprintf("%d", (int)(i * ystep) );
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  ypZeroLine + i * getAtomGridHeight()), str);
    }
}

void CapitalAverageDiffGrid::drawTopInfo()
{
    int currentDayAtMouse = (mousePoint.x() - getMarginLeft()) * totalDay / getGridWidth() + beginDay;
    if( currentDayAtMouse >= endDay) {
        currentDayAtMouse = endDay;
    } else if (currentDayAtMouse <= beginDay) {
        currentDayAtMouse = beginDay;
    }
    double diff = mDataFile->kline[currentDayAtMouse].capitalAvgDiff;

    std::stringstream stream;
    stream << "Diff: " << std::fixed << std::setprecision(2) << diff;

    QPainter painter(this);
    QFont font;
    font.setPointSize(11);
    painter.setFont(font);
    QPen     pen;
    pen.setColor(Qt::yellow);
    painter.setPen(pen);

    QRect rectText(5 + getMarginLeft(), 3, 150, topInfoHeight);
    painter.drawText(rectText, QString(stream.str().c_str()));
}

void CapitalAverageDiffGrid::initial()
{
    maxVolume = 0;
}

//获得画图所需的各项指标
void CapitalAverageDiffGrid::getIndicator()
{
    maxVolume = 0.0;
    for(int i=beginDay;i<endDay;++i)
    {
        double volume = std::fabs(mDataFile->kline[i].capitalAvgDiff);
        if (volume > maxVolume)
            maxVolume = volume;
    }
    ypZeroLine = getWidgetHeight() - (getGridHeight() / 2);
}

void CapitalAverageDiffGrid::drawVolume()
{
    QPainter painter(this);
    QPen     pen;

    double xstep = getGridWidth() / totalDay;
    double yscale = getGridHeight() / 2.0 / maxVolume;

    for( int i= beginDay;i<endDay;++i)
    {
        if(mDataFile->kline[i].capitalAvgDiff > 0.0)
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

        double temp = mDataFile->kline[i].capitalAvgDiff;

        pen.setWidth(1);
        painter.setPen(pen);

        p1.setX( getMarginLeft() + xstep *(i - beginDay) );
        p1.setY( ypZeroLine - (temp ) *yscale - getMarginBottom());
        p2.setX(getMarginLeft() + xstep *(i - beginDay) + lineWidth);
        p2.setY( ypZeroLine - (temp ) *yscale - getMarginBottom());
        p3.setX( getMarginLeft() + xstep *(i - beginDay) );
        p3.setY( ypZeroLine  - getMarginBottom() );
        p4.setX( getMarginLeft() + xstep *(i - beginDay) + lineWidth);
        p4.setY( ypZeroLine  - getMarginBottom() );

        painter.drawLine(p1,p2);
        painter.drawLine(p1,p3);
        painter.drawLine(p2,p4);
        painter.drawLine(p3,p4);
    }
}

void CapitalAverageDiffGrid::drawCross()
{
    drawCrossVerLine();
    drawCrossHorLine();
}

void CapitalAverageDiffGrid::drawCrossVerLine()
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

void CapitalAverageDiffGrid::drawCrossHorLine()
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

void CapitalAverageDiffGrid::drawCross2()
{
    drawMouseMoveCrossHorLine();
    drawMouseMoveCrossVerLine();
}

void CapitalAverageDiffGrid::drawMouseMoveCrossVerLine()
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

void CapitalAverageDiffGrid::drawMouseMoveCrossHorLine()
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
