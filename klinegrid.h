﻿#ifndef KLINEGRID_H
#define KLINEGRID_H

#include "datawidget.h"
#include "datafile.h"
#include "showdetail.h"
#include "marketdatasplitter.h"

#include <QPoint>
#include <QString>

class KLineGrid : public DataWidget
{
    Q_OBJECT

public:
    explicit KLineGrid(MarketDataSplitter *parent = nullptr, DataFile* dataFile = nullptr);
    void virtual paintEvent(QPaintEvent* event);
    void virtual keyPressEvent(QKeyEvent* event);
    void virtual mouseMoveEvent(QMouseEvent* event);
    void virtual mousePressEvent(QMouseEvent* event);
    void virtual resizeEvent(QResizeEvent* event);
    ~KLineGrid();

    void initial();
    void drawLine();
    void getIndicator();
    void drawYtick();
    void drawKline();


    //键盘按下后画的十字线
    void drawCross();
    void drawCrossVerLine();
    void drawCrossHorLine();
    void drawTips();

    //键盘没按下画的十字线
    void drawCross2();
    void drawMouseMoveCrossVerLine();
    void drawMouseMoveCrossHorLine();
    void drawTips2();


    //画均线
    void drawAverageLine(int day);

private:
    void drawDataDetailBox();
    void updateDataDetailBox();

public slots:
    void keyPressEventFromParent(QKeyEvent* event);
    void mouseMoveEventFromParent(QMouseEvent* event);
    void mousePressEventFromParent(QMouseEvent* event);

private:
    ShowDetail* mShowDrtail;


    //画k线的起始日期和终止日期
    int beginDay;
    int endDay;
    int totalDay;
    int currentDay;


    //当前要画的k线日期中的最高价，最低价，最大成交量
    double highestBid;
    double lowestBid;
    double maxVolume;

    //x轴和y轴的缩放比
    double xscale;
    double yscale;

    //是否显示十字线
    bool bCross = false;


    //鼠标位置
    QPoint mousePoint;
    bool isUnderMouse = false;

    //画笔的线宽
    int lineWidth;


    //键盘是否按下
    bool isKeyDown = false;



    //是否画均线
    bool isDrawAverageLine = true;

signals:



};

#endif // KLINEGRID_H
