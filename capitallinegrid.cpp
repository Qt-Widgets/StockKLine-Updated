#include <QMessageBox>
#include <QPainter>
#include <QPen>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QVector>
#include <QWidget>
#include <QLineEdit>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "mainwindow.h"
#include "capitallinegrid.h"

CapitalLineGrid::CapitalLineGrid(MarketDataSplitter *parent, DataFile* dataFile)
    : DataWidget(parent, dataFile), backtestingConfig(BacktestingConfig::instance())
{
    //开启鼠标追踪
    setMouseTracking(true);

    initial();
}

CapitalLineGrid::~CapitalLineGrid()
{

}

void CapitalLineGrid::initial()
{
    //开启鼠标追踪
    setMouseTracking(true);

    //初始化一些成员变量
    highestBid = 0;
    lowestBid = 1000;
    maxVolume = 0;
}

void CapitalLineGrid::keyPressEvent(QKeyEvent *event)
{
    static_cast<MarketDataSplitter*>(parent())->childKeyPressEvent(event);
}

void CapitalLineGrid::mouseMoveEvent(QMouseEvent *event)
{
    static_cast<MarketDataSplitter*>(parent())->childMouseMoveEvent(event);
}

void CapitalLineGrid::mousePressEvent(QMouseEvent *event)
{
    static_cast<MarketDataSplitter*>(parent())->childMousePressEvent(event);
}

void CapitalLineGrid::mouseReleaseEvent(QMouseEvent *event)
{
    static_cast<MarketDataSplitter*>(parent())->childMouseReleaseEvent(event);
}

void CapitalLineGrid::paintEvent(QPaintEvent *event)
{
    AutoGrid::paintEvent(event);
    drawLine();
}

void CapitalLineGrid::avgIntervalChanged()
{
    int avgIntervalUpdate = topBacktestingMenu->getAvgIntervalEdit()->text().toInt();
    if (avgIntervalUpdate > 0 && avgIntervalUpdate != avgInterval) {
        avgInterval = avgIntervalUpdate;
        mDataFile->capitalAverageLinePeriod[0] = avgInterval;
        mDataFile->calCapitalAverageLine();
    }
}

void CapitalLineGrid::setBacktestingTab(BacktestingTab* tab)
{
    this->backtestingTab = tab;
}

//void CapitalLineGrid::backtestingConfigChanged()
//{
//    backtestingConfig->capitalPeriod = topBacktestingMenu->getCapitalPeriodEdit()->text().toInt();
//    backtestingConfig->negThreshold1 = topBacktestingMenu->getNegThreshold1Edit()->text().toDouble() / 10000.0;
//    backtestingConfig->negThreshold2 = topBacktestingMenu->getNegThreshold2Edit()->text().toDouble() / 10000.0;
//    backtestingConfig->negThreshold3 = topBacktestingMenu->getNegThreshold3Edit()->text().toDouble() / 10000.0;
//    backtestingConfig->posThreshold1 = topBacktestingMenu->getPosThreshold1Edit()->text().toDouble() / 10000.0;
//    backtestingConfig->posThreshold2 = topBacktestingMenu->getPosThreshold2Edit()->text().toDouble() / 10000.0;
//    backtestingConfig->posThreshold3 = topBacktestingMenu->getPosThreshold3Edit()->text().toDouble() / 10000.0;
//    backtestingConfig->posThreshold4 = topBacktestingMenu->getPosThreshold4Edit()->text().toDouble() / 10000.0;
//    backtestingConfig->posThreshold5 = topBacktestingMenu->getPosThreshold5Edit()->text().toDouble() / 10000.0;

//    backtestingConfig->negLotThreshold1 = topBacktestingMenu->getNegLotThreshold1Edit()->text().toInt();
//    backtestingConfig->negLotThreshold2 = topBacktestingMenu->getNegLotThreshold2Edit()->text().toInt();
//    backtestingConfig->negLotThreshold3 = topBacktestingMenu->getNegLotThreshold3Edit()->text().toInt();

//    backtestingConfig->totalAdjLot =
//            backtestingConfig->negLotThreshold1 +
//            backtestingConfig->negLotThreshold2 +
//            backtestingConfig->negLotThreshold3;

//    backtestingConfig->posLotThreshold1 = topBacktestingMenu->getPosLotThreshold1Edit()->text().toInt();
//    backtestingConfig->posLotThreshold2 = topBacktestingMenu->getPosLotThreshold2Edit()->text().toInt();
//    backtestingConfig->posLotThreshold3 = topBacktestingMenu->getPosLotThreshold3Edit()->text().toInt();
//    backtestingConfig->posLotThreshold4 = topBacktestingMenu->getPosLotThreshold4Edit()->text().toInt();
//    //backtestingConfig->posLotThreshold5 = topBacktestingMenu->getPosLotThreshold5Edit()->text().toInt();

//    backtestingConfig->baseLot = topBacktestingMenu->getBaseLotEdit()->text().toInt();
//    backtestingConfig->enableCapitalAjdustment = topBacktestingMenu->getEnableCapitalAjdustmentCheckBox()->isChecked();

//    backtestingDriver.test();
//    backtestingTab->loadData(); // 重新加载数据

//    std::cout << "backtestingConfigChanged" << std::endl;
//}

void CapitalLineGrid::backtestingConfigChanged()
{
    backtestingConfig->capitalPeriod = topBacktestingMenu->getCapitalPeriodEdit()->text().toInt();

    // converted to percentage values
    backtestingConfig->addLotDiffThreshold1 = topBacktestingMenu->getAddLotDiffThreshold1Edit()->text().toDouble();
    backtestingConfig->addLotBacktrackThreshold1 = topBacktestingMenu->getAddLotBacktrackThreshold1Edit()->text().toDouble();

    backtestingConfig->decLotDiffThreshold1 = topBacktestingMenu->getDecLotDiffThreshold1Edit()->text().toDouble();
    backtestingConfig->decLotDiffThreshold2 = topBacktestingMenu->getDecLotDiffThreshold2Edit()->text().toDouble();

    backtestingConfig->baseLot = topBacktestingMenu->getBaseLotEdit()->text().toInt();

    backtestingDriver.test();
    backtestingTab->loadData(); // 重新加载数据

    std::cout << "backtestingConfigChanged" << std::endl;
}

void CapitalLineGrid::trackTopBacktestingMenu(TopBacktestingSimpleExMenu* topBacktestingMenu)
{
    this->topBacktestingMenu = topBacktestingMenu;
    // ugly, but working
    connect(topBacktestingMenu->getAvgIntervalEdit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::avgIntervalChanged);
    connect(topBacktestingMenu->getRunTestButton(), &QPushButton::clicked, this, &CapitalLineGrid::backtestingConfigChanged);

//    connect(topBacktestingMenu->getCapitalPeriodEdit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getNegThreshold3Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getNegThreshold2Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getNegThreshold1Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getPosThreshold1Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getPosThreshold2Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getPosThreshold3Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getPosThreshold4Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);
//    connect(topBacktestingMenu->getPosThreshold5Edit(), &QLineEdit::editingFinished, this, &CapitalLineGrid::backtestingConfigChanged);

}

void CapitalLineGrid::drawLine()
{
    //获取y轴指标
    getIndicator();

    //显示y轴价格
    drawYtick();

    //画十字线
    if( !isKeyDown && bCross)
    {
        drawCross2();
    }

    if(isKeyDown && bCross)
    {
        drawCross();
    }

    updateTopAverageLineInfo();

    drawAverageLine();
    drawAverageSimpleLine();
    drawCapitalLine();
    drawCapitalSimpleLine();
}


void CapitalLineGrid::getIndicator()
{
    lineWidth = getGridWidth() / totalDay;

    highestBid = 0;
    lowestBid = std::numeric_limits<double>::max();
    maxVolume = 0;

    highestCapital = 0.0;
    lowestCapital = std::numeric_limits<double>::max();
    highestCapitalSimple = 0.0;
    lowestCapitalSimple = std::numeric_limits<double>::max();

    for( int i= beginDay;i<endDay;++i)
    {
        if (mDataFile->kline[i].capital > highestCapital) {
            highestCapital = mDataFile->kline[i].capital;
        }
        if (mDataFile->kline[i].capital < lowestCapital) {
            lowestCapital = mDataFile->kline[i].capital;
        }

        if (mDataFile->kline[i].capitalForSimpleStrategy > highestCapitalSimple) {
            highestCapitalSimple = mDataFile->kline[i].capitalForSimpleStrategy;
        }
        if (mDataFile->kline[i].capitalForSimpleStrategy < lowestCapitalSimple) {
            lowestCapitalSimple = mDataFile->kline[i].capitalForSimpleStrategy;
        }
    }
}

void CapitalLineGrid::drawYtick()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);

    double ystep = (highestCapital - lowestCapital)/ getHGridNum();
    QString str;

    if( 0 == getHGridNum() )
    {
        str.sprintf("%.2f", lowestCapital);
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  getWidgetHeight() - getMarginBottom() ),
                          str);
        str.sprintf("%.2f", highestCapital);
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  getMarginTop() ),
                          str);
        return;
    }

    for(int i=0; i<getHGridNum(); ++i)
    {
        str.sprintf("%.2f", lowestCapital + i * ystep);
        painter.drawText( QPoint( getWidgetWidth() - getMarginRight() + 10,
                                  getWidgetHeight() - getMarginBottom() - i * getAtomGridHeight()),
                          str);
    }
}

void CapitalLineGrid::drawCross()
{

    drawCrossVerLine();
    drawCrossHorLine();
    drawTips();
}

void CapitalLineGrid::drawCrossVerLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    double xstep = getGridWidth() / totalDay;
    double xPos = getMarginLeft();
    while( mousePoint.x() - xPos > xstep ) {
        xPos += xstep;
    }
    xPos += 0.5 * lineWidth;
    QLine horline(xPos, getMarginTop(), xPos, getWidgetHeight() - getMarginBottom());
    painter.drawLine(horline);
}

void CapitalLineGrid::drawCrossHorLine()
{
    if(!isUnderMouse)
        return;

    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FFFFFF"));
    pen.setWidth(1);
    painter.setPen(pen);

    currentDay = ( mousePoint.x() - getMarginLeft() ) * totalDay / getGridWidth() + beginDay;
    double yPos =  ( mDataFile->kline[currentDay].capital - lowestCapital ) * yscale ;

    QLine verline ( getMarginLeft(),getWidgetHeight()-getMarginBottom()-yPos,
                    getWidgetWidth()-getMarginRight(),getWidgetHeight()-getMarginBottom()-yPos);
    painter.drawLine(verline);
}

void CapitalLineGrid::drawTips()
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
    double yval = mDataFile->kline[currentDay].capital;

    double yPos = (mDataFile->kline[currentDay].capital - lowestCapital) * yscale ;
    yPos = getWidgetHeight() - getMarginBottom() - yPos;

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

void CapitalLineGrid::updateTopAverageLineInfo()
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
    for (int i = 0; i < mDataFile->capitalAverageLineCount; ++i) {
        pen.setColor(mDataFile->averageLineColors[i]);
        painter.setPen(pen);
        std::stringstream stream;
        stream << "MA" << std::fixed << std::setprecision(2) << mDataFile->capitalAverageLinePeriod[i]
                  << " " << std::fixed << std::setprecision(2) << mDataFile->kline[currentDayAtMouse].capitalAvgs[i];

        QRect rectText(x, 3, x + 100, topAverageLineInfoHeight);
        painter.drawText(rectText, QString(stream.str().c_str()));
        x += 100 + 20;
    }
}

void CapitalLineGrid::drawMouseMoveCrossVerLine()
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

void CapitalLineGrid::drawMouseMoveCrossHorLine()
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

void CapitalLineGrid::drawCross2()
{
    drawMouseMoveCrossHorLine();
    drawMouseMoveCrossVerLine();
    drawTips2();
}

void CapitalLineGrid::drawTips2()
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

    double yval =  highestCapital - ( mousePoint.y() - getMarginTop() ) / yscale;
    double yPos = mousePoint.y();

    int iTipsWidth = 70;
    int iTipsHeight = 30;

    QString str;

    QRect rect( getWidgetWidth() - getMarginRight(),
                yPos - iTipsHeight/2,iTipsWidth,iTipsHeight);
    painter.drawRect(rect);

    QRect rectText( getWidgetWidth() - getMarginRight() + iTipsWidth/4,
                yPos - iTipsHeight/4,iTipsWidth,iTipsHeight);
    painter.drawText(rectText, str.sprintf("%.2f",yval));
}

void CapitalLineGrid::drawCapitalLine()
{
    if( beginDay < 0)
        return;

    double capitalYScale = getGridHeight() / (highestCapital - lowestCapital);
    QVector<QPoint> point;
    QPoint temp;
    double xstep = getGridWidth() / totalDay;

    for(int i = beginDay; i < endDay; ++i) {
        temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
        temp.setY(getWidgetHeight() - (mDataFile->kline[i].capital - lowestCapital) * capitalYScale - getMarginBottom());
        point.push_back(temp);
    }

    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::darkGreen);
    painter.setPen(pen);
    QPolygon polykline(point);
    painter.drawPolyline(polykline);

    QPen tradingSignalPen;
    for (int i = beginDay; i < endDay; i++) {
        drawTradingSignal(i, getMarginLeft() + xstep * (i - beginDay) + 0.5 * lineWidth, painter, tradingSignalPen);
    }
}

void CapitalLineGrid::drawTradingSignal(int index, int x, QPainter& painter, QPen& pen)
{
    QPen originalPen = painter.pen();
    QString signal = mDataFile->kline[index].adjustmentSignal;
    if (signal.size() == 0) {
        return;
    }
    pen.setColor(Qt::white);
    painter.setPen(pen);
    painter.drawText(QPoint( x - 2, getMarginTop() + 35), signal);

    painter.setPen(originalPen);
}

void CapitalLineGrid::drawCapitalSimpleLine()
{
    if( beginDay < 0)
        return;

    double capitalYScale = getGridHeight() / (highestCapitalSimple - lowestCapitalSimple);
    QVector<QPoint> point;
    QPoint temp;
    double xstep = getGridWidth() / totalDay;

    for(int i = beginDay; i < endDay; ++i) {
        temp.setX(getMarginLeft() + xstep *(i - beginDay) + 0.5*lineWidth);
        temp.setY(getWidgetHeight() - (mDataFile->kline[i].capitalForSimpleStrategy - lowestCapitalSimple) * capitalYScale - getMarginBottom());
        point.push_back(temp);
    }

    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    QPolygon polykline(point);
    painter.drawPolyline(polykline);
}

void CapitalLineGrid::drawAverageLine()
{

    //y轴缩放
    yscale = getGridHeight() / (highestCapital - lowestCapital) ;
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

    for (int i = 0; i < mDataFile->capitalAverageLineCount; i++) {
        point.clear();
        for (int j = beginDay; j < endDay; j++) {
            if (mDataFile->kline[j].capitalAvgs[i] <= 0.0)
                continue;
            temp.setX(getMarginLeft() + xstep * (j - beginDay) + 0.5 * lineWidth);
            temp.setY(getWidgetHeight() - (mDataFile->kline[j].capitalAvgs[i] - lowestCapital) * yscale - getMarginBottom());
            point.push_back(temp);
        }

        QPainter painter(this);
        QPen     pen;

        pen.setColor(mDataFile->capitalAverageLineColors[i]);
        painter.setPen(pen);
        QPolygon polykline(point);
        painter.drawPolyline(polykline);
    }
}

void CapitalLineGrid::drawAverageSimpleLine()
{

    //y轴缩放
    double yscale = getGridHeight() / (highestCapitalSimple - lowestCapitalSimple) ;
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

    for (int i = 0; i < mDataFile->capitalAverageLineCount; i++) {
        point.clear();
        for (int j = beginDay; j < endDay; j++) {
            if (mDataFile->kline[j].capitalAvgsForSimpleStrategy[i] <= 0.0)
                continue;
            temp.setX(getMarginLeft() + xstep * (j - beginDay) + 0.5 * lineWidth);
            temp.setY(getWidgetHeight() - (mDataFile->kline[j].capitalAvgsForSimpleStrategy[i] - lowestCapitalSimple) * yscale - getMarginBottom());
            point.push_back(temp);
        }

        QPainter painter(this);
        QPen     pen;

        pen.setColor(Qt::red);
        painter.setPen(pen);
        QPolygon polykline(point);
        painter.drawPolyline(polykline);
    }
}
