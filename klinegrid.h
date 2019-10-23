﻿#ifndef KLINEGRID_H
#define KLINEGRID_H

#include "datawidget.h"
#include "datafile.h"
#include "showdetail.h"
#include "marketdatasplitter.h"
#include "backtesting/backtestingconfig.h"
#include "marketdatatab.h"

#include <QPoint>
#include <QString>

class KLineGrid : public DataWidget
{
    Q_OBJECT

public:
    explicit KLineGrid(MarketDataSplitter *parent = nullptr, DataFile* dataFile = nullptr);
    void virtual paintEvent(QPaintEvent* event) override;
    void virtual keyPressEvent(QKeyEvent* event) override;
    void virtual mouseMoveEvent(QMouseEvent* event) override;
    void virtual mousePressEvent(QMouseEvent* event) override;
    void virtual mouseReleaseEvent(QMouseEvent* event) override;
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
    void drawAverageLine();
    void drawCapitalLine();

    void setMarketDataTab(MarketDataTab *value);

private:
    void drawDataDetailBox();
    void updateDataDetailBox();
    void updateTopAverageLineInfo();
    void drawTradingSignal(int index, int x, QPainter& painter, QPen& pen);

private:
    ShowDetail* mShowDrtail;

    //当前要画的k线日期中的最高价，最低价，最大成交量
    double highestBid;
    double lowestBid;
    double maxVolume;
    double highestCapital;
    double lowestCapital;

    //x轴和y轴的缩放比
    double xscale;
    double yscale;

    //画笔的线宽
    int lineWidth;

    //是否画均线
    bool isDrawAverageLine = true;

    int topAverageLineInfoHeight = 20;

    int currentProductIndex;
    MarketDataTab* marketDataTab;
    BacktestingConfig* backtestingConfig;

signals:

};

#endif // KLINEGRID_H
