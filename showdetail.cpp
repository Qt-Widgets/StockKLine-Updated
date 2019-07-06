#include "showdetail.h"
#include "mainwindow.h"
#include <QPainter>
#include <QFont>

ShowDetail::ShowDetail(QWidget* parent) : QDialog(parent, Qt::FramelessWindowHint)
{

    QFont ft;
    ft.setPointSize(10);
    this->setFont(ft);
    this->setStyleSheet("border: 1px solid white");
    this->setModal(false);
    this->setFixedSize(boxWidth, 200);
    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
}


void ShowDetail::drawBK()
{
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Window,QColor("#000000"));
    this->setPalette(palette);
}

void ShowDetail::paintEvent(QPaintEvent *event)
{
    drawBK();
    drawStr();
}

void ShowDetail::drawStr()
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);

    int count = 0;

    pen.setColor(timeColor);
    painter.setPen(pen);
    QRect rectTime2(dataMarginLeft, 4, boxWidth, dataHeight);
    QString str = time.mid(1,10).remove("-");
    painter.drawText(rectTime2,str);

//    count++;
//    pen.setColor(QColor("#FFFFFF"));
//    painter.setPen(pen);
//    QRect rectTime3(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
//    painter.drawText(rectTime3,QStringLiteral("currentValue"));

//    count++;
//    pen.setColor(currentPriceColor);
//    painter.setPen(pen);
//    QRect rectTime4(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
//    painter.drawText(rectTime4,str.sprintf("%.2f",currentPrice));

    count++;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime5(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime5,QStringLiteral("开盘"));

    count++;
    pen.setColor(openingPriceColor);
    painter.setPen(pen);
    QRect rectTime6(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime6,str.sprintf("%.2f",openingPrice));

    count++;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime7(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime7,QStringLiteral("最高"));

    count++;
    pen.setColor(highestBidColor);
    painter.setPen(pen);
    QRect rectTime8(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime8,str.sprintf("%.2f",highestBid));

    count++;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime9(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime9,QStringLiteral("最低"));

    count++;
    pen.setColor(lowestBidColor);
    painter.setPen(pen);
    QRect rectTime10(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime10,str.sprintf("%.2f",lowestBid));

    count++;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime11(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime11,QStringLiteral("收盘"));

    count++;
    pen.setColor(closeingPriceColor);
    painter.setPen(pen);
    QRect rectTime12(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime12,str.sprintf("%.2f",closeingPrice));

    count++;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime13(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime13,QStringLiteral("总手"));

    count++;
    pen.setColor(amountOfIncreaseColor);
    painter.setPen(pen);
    QRect rectTime14(dataMarginLeft, 4 + (dataHeight + dataSpacing) * count, boxWidth, dataHeight);
    painter.drawText(rectTime14,str.sprintf("%.2f%",amountOfIncrease));



    /*
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime15(20,440,100,30);
    painter.drawText(rectTime15,QStringLiteral("amountOfAmplitude"));


    pen.setColor(amountOfAmplitudeColor);
    painter.setPen(pen);
    QRect rectTime16(20,470,100,30);
    painter.drawText(rectTime16,str.sprintf("%.2f%",amountOfAmplitude));





    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime17(20,500,100,30);
    painter.drawText(rectTime17,QStringLiteral("totalVolume"));




    pen.setColor(totalVolumeColor);
    painter.setPen(pen);
    QRect rectTime18(20,530,100,30);
    str = totalVolume;
    str = str.mid(1,str.length());
    str = str.mid(0,str.length()-1);
    painter.drawText(rectTime18,str);




    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime19(20,560,100,30);
    painter.drawText(rectTime19,QStringLiteral("totalAmount"));



    pen.setColor(totalAmountColor);
    painter.setPen(pen);
    QRect rectTime20(20,590,100,30);

    str = totalAmount;
    str = str.mid(1,str.length());
    str = str.mid(0,str.length()-1);
    painter.drawText(rectTime20,str);




    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);
    QRect rectTime21(20,620,100,30);
    painter.drawText(rectTime21,tr("turnoverRate"));



    pen.setColor(turnoverRateColor);
    painter.setPen(pen);
    QRect rectTime22(20,650,100,30);
    painter.drawText(rectTime22,str.sprintf("%.2f",turnoverRate));*/



//    QRect rectTime4( 20,50,100,30);
//    painter.drawText(rectTime4,str.sprintf("%.2f",currentPrice));

}

void ShowDetail::receiveParams(QString time,QColor timeColor,
                               double currentPrice,QColor currentPriceColor,
                               double openingPrice,QColor openingPriceColor,
                               double highestBid,QColor highestBidColor,
                               double lowestBid,QColor lowestBidColor,
                               double closeingPrice,QColor closeingPriceColor,
                               double amountOfIncrease,QColor amountOfIncreaseColor,
                               double amountOfAmplitude,QColor amountOfAmplitudeColor,
                               QString totalVolume,QColor totalVolumeColor,
                               QString totalAmount,QColor totalAmountColor,
                               double turnoverRate,QColor turnoverRateColor)
{
    this->time = time;
    this->currentPrice = currentPrice;
    this->openingPrice = openingPrice;
    this->highestBid = highestBid;
    this->lowestBid = lowestBid;
    this->closeingPrice = closeingPrice;
    this->amountOfIncrease = amountOfIncrease;
    this->amountOfAmplitude = amountOfAmplitude;
    this->totalVolume = totalVolume;
    this->totalAmount = totalAmount;
    this->turnoverRate = turnoverRate;


    this->timeColor = timeColor;
    this->currentPriceColor = currentPriceColor;
    this->openingPriceColor = openingPriceColor;
    this->highestBidColor = highestBidColor;
    this->lowestBidColor = lowestBidColor;
    this->closeingPriceColor = closeingPriceColor;
    this->amountOfIncreaseColor = amountOfIncreaseColor;
    this->amountOfAmplitudeColor = amountOfAmplitudeColor;
    this->totalVolumeColor = totalVolumeColor;
    this->totalAmountColor = totalAmountColor;
    this->turnoverRateColor = turnoverRateColor;

    update();
}
