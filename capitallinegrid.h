#ifndef CAPITALLINEGRID_H
#define CAPITALLINEGRID_H

#include "datawidget.h"
#include "datafile.h"
#include "marketdatasplitter.h"
#include "topbacktestingsimpleexmenu.h"
#include "backtestingtab.h"
#include "backtesting/backtestingconfig.h"
#include "backtesting/backtestingdriverinterface.h"

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
    void drawAverageSimpleLine();
    void drawCapitalLine();
    void drawCapitalSimpleLine();

    void trackTopBacktestingMenu(TopBacktestingSimpleExMenu* topBacktestingMenu);
    void setBacktestingTab(BacktestingTab* tab);

private:
    void drawTradingSignal(int index, int x, QPainter& painter, QPen& pen);

public slots:
    void avgIntervalChanged();
    void backtestingConfigChanged();
    void productChanged(int index);

private:
    void updateTopAverageLineInfo();

private:
    //当前要画的k线日期中的最高价，最低价，最大成交量
    double highestBid;
    double lowestBid;
    double maxVolume;
    double highestCapital;
    double lowestCapital;

    double highestCapitalSimple;
    double lowestCapitalSimple;

    //x轴和y轴的缩放比
    double xscale;
    double yscale;

    //画笔的线宽
    int lineWidth;

    //是否画均线
    bool isDrawAverageLine = true;

    int topAverageLineInfoHeight = 20;

    TopBacktestingSimpleExMenu* topBacktestingMenu;
    BacktestingTab* backtestingTab;
    BacktestingConfig* backtestingConfig;
    int avgInterval = 250;

    std::shared_ptr<BacktestingDriverInterface> backtestingDriver;

signals:

};

#endif // CAPITALLINEGRID_H
