#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QVector>
#include <QWidget>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "mainwindow.h"
#include "klinegrid.h"

KLineGrid::KLineGrid(MarketDataSplitter *parent, DataFile* dataFile)
    : DataWidget(parent, dataFile)
{
    //开启鼠标追踪
    setMouseTracking(true);

    initial();
}

KLineGrid::~KLineGrid()
{
    delete mShowDrtail;
    mShowDrtail = nullptr;
}

void KLineGrid::initial()
{
    //开启鼠标追踪
    setMouseTracking(true);

    //初始化一些成员变量
    highestBid = 0;
    lowestBid = 1000;
    maxVolume = 0;

    //构造详细数据展示页面
    mShowDrtail = new ShowDetail(this);
}

void KLineGrid::keyPressEvent(QKeyEvent *event)
{
    static_cast<MarketDataSplitter*>(parent())->childKeyPressEvent(event);
}

void KLineGrid::mouseMoveEvent(QMouseEvent *event)
{
    static_cast<MarketDataSplitter*>(parent())->childMouseMoveEvent(event);
}

void KLineGrid::mousePressEvent(QMouseEvent *event)
{
    static_cast<MarketDataSplitter*>(parent())->childMousePressEvent(event);
}

void KLineGrid::paintEvent(QPaintEvent *event)
{
    AutoGrid::paintEvent(event);

    //画k线
    drawLine();
    drawDataDetailBox();
}

void KLineGrid::drawDataDetailBox()
{
    if (bCross) {
        //构造详细数据展示页面
        //QPoint klineOriginPos = this->pos();
        //mShowDrtail->move(klineOriginPos.x() + 2, klineOriginPos.y() + getMarginTop());
        mShowDrtail->move(2, 7);
        mShowDrtail->show();
    } else {
        mShowDrtail->hide();
    }
}


void KLineGrid::drawLine()
{
    //获取y轴指标
    getIndicator();

    //显示y轴价格
    drawYtick();

    //画k线
    drawKline();


    //画十字线
    if( !isKeyDown && bCross)
    {
        drawCross2();
    }

    if(isKeyDown && bCross)
    {
        drawCross();
    }

    updateDataDetailBox();
    updateTopAverageLineInfo();


    drawAverageLine();
    //drawAverageLine(10);
    //drawAverageLine(20);
    //drawAverageLine(30);
    //drawAverageLine(60);

}


void KLineGrid::getIndicator()
{

    highestBid = 0;
    lowestBid = std::numeric_limits<double>::max();
    maxVolume = 0;

    for( int i= beginDay;i<endDay;++i)
    {
        if( mDataFile->kline[i].highestBid > highestBid )
            highestBid = mDataFile->kline[i].highestBid;
        if( mDataFile->kline[i].lowestBid < lowestBid )
            lowestBid = mDataFile->kline[i].lowestBid;
//        if( mDataFile->kline[i].totalVolume.toFloat() > maxVolume )
//            maxVolume = mDataFile->kline[i].totalVolume.toFloat();
    }
}

void KLineGrid::drawYtick()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);

    double ystep = (highestBid - lowestBid)/ getHGridNum();
    QString str;


    if( 0 == getHGridNum() )
    {
        str.sprintf("%.2f",lowestBid);
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  getWidgetHeight() - getMarginBottom() ),
                          str);
        str.sprintf("%.2f",highestBid);
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  getMarginTop() ),
                          str);
        return;
    }

    for( int i=0;i<getHGridNum();++i)
    {
        str.sprintf("%.2f",lowestBid+ i*ystep);
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  getWidgetHeight() - getMarginBottom() - i*getAtomGridHeight()),
                          str);
    }
}

void KLineGrid::drawKline()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);

    QPen tradingSignalPen;


    if (beginDay <0)
        return;

    //y轴缩放
    yscale = getGridHeight() / (highestBid -lowestBid ) ;

    //画笔的线宽
    lineWidth;

    //画线连接的两个点
    QPoint p1;
    QPoint p2;

    QPoint p3;
    QPoint p4;

    double xstep = getGridWidth() / totalDay;

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



        //阴线

        if( mDataFile->kline[i].openingPrice > mDataFile->kline[i].closeingPrice )
        {
            //画开盘与收盘之间的粗实线
            pen.setWidth(lineWidth);
            painter.setPen(pen);
            p1.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p1.setY( getWidgetHeight() - (mDataFile->kline[i].openingPrice - lowestBid) *yscale - getMarginBottom() + 0.5*lineWidth);
            p2.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p2.setY( getWidgetHeight() - (mDataFile->kline[i].closeingPrice - lowestBid) *yscale - getMarginBottom() - 0.5*lineWidth);
            painter.drawLine(p1,p2);


            //画最高价与最低价之间的细线
            pen.setWidth(1);
            painter.setPen(pen);
            p1.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p1.setY( getWidgetHeight() - (mDataFile->kline[i].highestBid - lowestBid) *yscale - getMarginBottom());
            p2.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p2.setY( getWidgetHeight() - (mDataFile->kline[i].lowestBid - lowestBid) *yscale - getMarginBottom());
            painter.drawLine(p1,p2);


        }
        else
        {
            //像同花顺一样阳线画成空心的

            pen.setWidth(1);
            painter.setPen(pen);


            p1.setX( getMarginLeft() + xstep *(i - beginDay) );
            p1.setY( getWidgetHeight() - (mDataFile->kline[i].openingPrice - lowestBid) *yscale - getMarginBottom());

            p2.setX( getMarginLeft() + xstep *(i - beginDay) + lineWidth);
            p2.setY( getWidgetHeight() - (mDataFile->kline[i].openingPrice - lowestBid) *yscale - getMarginBottom());


            p3.setX( getMarginLeft() + xstep *(i - beginDay) );
            p3.setY( getWidgetHeight() - (mDataFile->kline[i].closeingPrice - lowestBid) *yscale - getMarginBottom());

            p4.setX( getMarginLeft() + xstep *(i - beginDay) + lineWidth);
            p4.setY( getWidgetHeight() - (mDataFile->kline[i].closeingPrice - lowestBid) *yscale - getMarginBottom());

            painter.drawLine(p1,p2);
            painter.drawLine(p1,p3);
            painter.drawLine(p2,p4);
            painter.drawLine(p3,p4);


            //画最高价与最低价之间的细线
            pen.setWidth(1);
            painter.setPen(pen);
            p1.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p1.setY( getWidgetHeight() - (mDataFile->kline[i].highestBid - lowestBid) *yscale - getMarginBottom());


            double y1,y2;
            if( mDataFile->kline[i].openingPrice > mDataFile->kline[i].closeingPrice )
            {
                y1 = mDataFile->kline[i].openingPrice;
                y2 = mDataFile->kline[i].closeingPrice;
            }
            else
            {
                y1 = mDataFile->kline[i].closeingPrice;
                y2 = mDataFile->kline[i].openingPrice;
            }

            p2.setX( getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p2.setY( getWidgetHeight() - (y1 - lowestBid) *yscale - getMarginBottom());
            p3.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p3.setY( getWidgetHeight() - (y2 - lowestBid) *yscale - getMarginBottom());
            p4.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
            p4.setY(getWidgetHeight() - (mDataFile->kline[i].lowestBid - lowestBid) *yscale - getMarginBottom());

            painter.drawLine(p1,p2);
            painter.drawLine(p3,p4);

            drawTradingSignal(i, getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth, painter, tradingSignalPen);
        }
    }
}

void KLineGrid::drawTradingSignal(int index, int x, QPainter& painter, QPen& pen)
{
    QPen originalPen = painter.pen();
    auto it = mDataFile->tradingSignalColors.find(mDataFile->kline[index].tradingSignal);
    if (it == mDataFile->tradingSignalColors.end()) {
        return;
    }
    Qt::GlobalColor color = it->second;
    pen.setColor(color);
    painter.setPen(pen);
    painter.drawText(QPoint( x - 2, getMarginTop() + 35), "*");

    painter.setPen(originalPen);
}

void KLineGrid::drawCross()
{

    drawCrossVerLine();
    drawCrossHorLine();
    drawTips();
}

void KLineGrid::drawCrossVerLine()
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

void KLineGrid::drawCrossHorLine()
{
    if(!isUnderMouse)
        return;

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);


    double yPos;
    currentDay = ( mousePoint.x() - getMarginLeft() ) * totalDay / getGridWidth() + beginDay;


    if( mDataFile->kline[currentDay].openingPrice < mDataFile->kline[currentDay].closeingPrice )
        yPos =  ( mDataFile->kline[currentDay].closeingPrice - lowestBid ) * yscale ;
    else
        yPos =  ( mDataFile->kline[currentDay].closeingPrice - lowestBid ) * yscale ;

    QLine verline ( getMarginLeft(),getWidgetHeight()-getMarginBottom()-yPos,
                    getWidgetWidth()-getMarginRight(),getWidgetHeight()-getMarginBottom()-yPos);
    painter.drawLine(verline);

}




void KLineGrid::drawTips()
{
    if (!isUnderMouse)
        return;

    QPainter painter(this);
    QPen     pen;
    QBrush brush(QColor(64,0,128));
    painter.setBrush(brush);
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);


    int currentDay = ( mousePoint.x() - getMarginLeft() ) * totalDay / getGridWidth() + beginDay;
    double yval = mDataFile->kline[currentDay].closeingPrice;


    double yPos;
    if( mDataFile->kline[currentDay].openingPrice < mDataFile->kline[currentDay].closeingPrice )
        yPos =  ( mDataFile->kline[currentDay].closeingPrice - lowestBid ) * yscale ;
    else
        yPos =  ( mDataFile->kline[currentDay].closeingPrice - lowestBid ) * yscale ;


    yPos = getWidgetHeight()-getMarginBottom()-yPos;

    int iTipsWidth = 60;
    int iTipsHeight = 30;

    QString str;

    QRect rect( getWidgetWidth() - getMarginRight(),
                yPos - iTipsHeight/2,iTipsWidth,iTipsHeight);
    painter.drawRect(rect);


    QRect rectText( getWidgetWidth() - getMarginRight() + iTipsWidth/4,
                yPos - iTipsHeight/4,iTipsWidth,iTipsHeight);
    painter.drawText(rectText, str.sprintf("%.2f",yval));
}

void KLineGrid::updateTopAverageLineInfo()
{
    int currentDayAtMouse = ( mousePoint.x() - getMarginLeft() ) * totalDay / getGridWidth() + beginDay;
    if( currentDayAtMouse >= endDay) {
        currentDayAtMouse = endDay;
    } else if (currentDayAtMouse <= beginDay) {
        currentDayAtMouse = beginDay;
    }

    QPainter painter(this);
    QFont font;
    font.setPointSize(11);
    painter.setFont(font);
    QPen     pen;

    int x = 5 + getMarginLeft();
    for (int i = 0; i < mDataFile->averageLineCount; ++i) {
        pen.setColor(mDataFile->averageLineColors[i]);
        painter.setPen(pen);
        std::stringstream stream;
        stream << "MA" << std::fixed << std::setprecision(2) << mDataFile->averageLinePeriod[i]
                  << " " << std::fixed << std::setprecision(2) << mDataFile->kline[currentDayAtMouse].averages[i];

        QRect rectText(x, 3, x + 100, topAverageLineInfoHeight);
        painter.drawText(rectText, QString(stream.str().c_str()));
        x += 100 + 20;
    }
}

void KLineGrid::updateDataDetailBox()
{
    int currentDayAtMouse = ( mousePoint.x() - getMarginLeft() ) * totalDay / getGridWidth() + beginDay;
    if( currentDayAtMouse >= endDay) {
        currentDayAtMouse = endDay;
    } else if (currentDayAtMouse <= beginDay) {
        currentDayAtMouse = beginDay;
    }

    QColor openingColor = mDataFile->kline[currentDayAtMouse].openingPrice > mDataFile->kline[std::max(currentDayAtMouse -1, 0)].openingPrice ?
                          QColor("#FF0000"):QColor("#00FF00");

    QColor highestColor = mDataFile->kline[currentDayAtMouse].highestBid > mDataFile->kline[std::max(currentDayAtMouse -1, 0)].closeingPrice ?
                QColor("#FF0000"):QColor("#00FF00");

    QColor lowestColor = mDataFile->kline[currentDayAtMouse].lowestBid > mDataFile->kline[std::max(currentDayAtMouse -1, 0)].closeingPrice ?
                QColor("#FF0000"):QColor("#00FF00");

    QColor closeingColor = mDataFile->kline[currentDayAtMouse].closeingPrice > mDataFile->kline[currentDayAtMouse ].openingPrice ?
                QColor("#FF0000"):QColor("#00FF00");

    QColor amountOfIncreaseColor = mDataFile->kline[currentDayAtMouse].amountOfIncrease > 0 ?
                QColor("#FF0000"):QColor("#00FF00");

    mShowDrtail->receiveParams(      mDataFile->kline[currentDayAtMouse].time,QColor("#FFFFFF"),
                                     mDataFile->kline[currentDayAtMouse].closeingPrice,QColor("#FF0000"),
                                     mDataFile->kline[currentDayAtMouse].openingPrice,openingColor,
                                     mDataFile->kline[currentDayAtMouse].highestBid,highestColor,
                                     mDataFile->kline[currentDayAtMouse].lowestBid,lowestColor,
                                     mDataFile->kline[currentDayAtMouse].closeingPrice,closeingColor,
                                     mDataFile->kline[currentDayAtMouse].amountOfIncrease,amountOfIncreaseColor,
                                     mDataFile->kline[currentDayAtMouse].amountOfAmplitude,QColor("#02E2F4"),
                                     mDataFile->kline[currentDayAtMouse].totalVolume,QColor("#02E2F4"),
                                     mDataFile->kline[currentDayAtMouse].totalAmount,QColor("#02E2F4"),
                                     mDataFile->kline[currentDayAtMouse].turnoverRate,QColor("#02E2F4")
                                     );
}

void KLineGrid::drawMouseMoveCrossVerLine()
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
    painter.drawLine(mousePoint.x(), getMarginTop(),
                     mousePoint.x(), getWidgetHeight() - getMarginBottom());
}

void KLineGrid::drawMouseMoveCrossHorLine()
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



void KLineGrid::drawCross2()
{
    drawMouseMoveCrossHorLine();
    drawMouseMoveCrossVerLine();
    drawTips2();
}

void KLineGrid::drawTips2()
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

    double yval =  highestBid - ( mousePoint.y() - getMarginTop() ) / yscale;
    double yPos = mousePoint.y();

    int iTipsWidth = 60;
    int iTipsHeight = 30;

    QString str;

    QRect rect( getWidgetWidth() - getMarginRight(),
                yPos - iTipsHeight/2,iTipsWidth,iTipsHeight);
    painter.drawRect(rect);


    QRect rectText( getWidgetWidth() - getMarginRight() + iTipsWidth/4,
                yPos - iTipsHeight/4,iTipsWidth,iTipsHeight);
    painter.drawText(rectText, str.sprintf("%.2f",yval));
}



void KLineGrid::drawAverageLine()
{

    //y轴缩放
    yscale = getGridHeight() / (highestBid -lowestBid ) ;
    //画笔的线宽
    lineWidth;
    //画线要连接的点
    QVector<QPoint> point;

    //临时点
    QPoint temp;

    //x轴步进
    double xstep = getGridWidth() / totalDay;



    if( beginDay < 0)
        return;


//    switch(day)
//    {
//    case 5:
//        for( int i= beginDay;i<endDay;++i)
//        {
//            if( mDataFile->kline[i].averageLine5 == 0)
//                continue;
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile->kline[i].averageLine5 - lowestBid) *yscale - getMarginBottom());
//            point.push_back(temp);
//        }
//        break;
//    case 10:
//        for( int i= beginDay;i<endDay;++i)
//        {
//            if( mDataFile->kline[i].averageLine10 == 0)
//                continue;
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile->kline[i].averageLine10 - lowestBid) *yscale - getMarginBottom());
//            point.push_back(temp);
//        }
//        break;
//    case 20:
//        for( int i= beginDay;i<endDay;++i)
//        {
//            if( mDataFile->kline[i].averageLine20 == 0)
//                continue;
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile->kline[i].averageLine20 - lowestBid) *yscale - getMarginBottom());
//            point.push_back(temp);
//        }
//        break;
//    case 30:
//        for( int i= beginDay;i<endDay;++i)
//        {
//            if( mDataFile->kline[i].averageLine30 == 0)
//                continue;
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile->kline[i].averageLine30 - lowestBid) *yscale - getMarginBottom());
//            point.push_back(temp);
//        }
//        break;
//    case 60:
//        for( int i= beginDay;i<endDay;++i)
//        {
//            if( mDataFile->kline[i].averageLine60 == 0)
//                continue;
//            temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
//            temp.setY(getWidgetHeight() - (mDataFile->kline[i].averageLine60 - lowestBid) *yscale - getMarginBottom());
//            point.push_back(temp);
//        }
//        break;
//    default:
//        break;
//    }

    for (int i = 0; i < mDataFile->averageLineCount; i++) {
        point.clear();
        for (int j = beginDay; j < endDay; j++) {
            if (mDataFile->kline[j].averages[i] <= 0.0)
                continue;
            temp.setX(getMarginLeft() + xstep *(j - beginDay) + 0.5*lineWidth);
            temp.setY(getWidgetHeight() - (mDataFile->kline[j].averages[i] - lowestBid) *yscale - getMarginBottom());
            point.push_back(temp);
        }

        QPainter painter(this);
        QPen     pen;

//        switch(i)
//        {
//        case 5:
//            pen.setColor(Qt::white);
//            break;
//        case 10:
//            pen.setColor(Qt::yellow);
//            break;
//        case 20:
//            pen.setColor(Qt::magenta);
//            break;
//        case 30:
//            pen.setColor(Qt::green);
//            break;
//        case 60:
//            pen.setColor(Qt::cyan);
//            break;
//        default:
//            pen.setColor(Qt::white);
//            break;
//        }
        pen.setColor(mDataFile->averageLineColors[i]);
        painter.setPen(pen);
        QPolygon polykline(point);
        painter.drawPolyline(polykline);
    }
}
