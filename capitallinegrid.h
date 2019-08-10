#ifndef CAPITALLINEGRID_H
#define CAPITALLINEGRID_H

#include "datawidget.h"
#include "datafile.h"
#include "marketdatasplitter.h"
#include "topbacktestingmenu.h"

#include <QPoint>
#include <QString>

class CapitalLineGrid : public DataWidget
{
    Q_OBJECT

public:
    explicit CapitalLineGrid(MarketDataSplitter *parent = nullptr, DataFile* dataFile = nullptr);
    void virtual paintEvent(QPaintEvent* event) override;
    void virtual keyPressEvent(QKeyEvent* event) override;
    void virtual mouseMoveEvent(QMouseEvent* event) override;
    void virtual mousePressEvent(QMouseEvent* event) override;
    void virtual mouseReleaseEvent(QMouseEvent* event) override;
    ~CapitalLineGrid() override;

    void initial();
    void drawLine();
    void getIndicator();
    void drawYtick();

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

    void trackTopBacktestingMenu(TopBacktestingMenu* topBacktestingMenu);

public slots:
    void avgIntervalChanged();

private:
    void updateTopAverageLineInfo();

private:
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

    TopBacktestingMenu* topBacktestingMenu;
    int avgInterval = 250;

signals:

};

#endif // CAPITALLINEGRID_H
