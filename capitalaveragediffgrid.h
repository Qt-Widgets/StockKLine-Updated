#ifndef CAPITALAVERAGEDIFF_H
#define CAPITALAVERAGEDIFF_H

#include "datawidget.h"
#include "datafile.h"
#include "marketdatasplitter.h"

#include <QPoint>

class CapitalAverageDiffGrid : public DataWidget
{
public:
    explicit CapitalAverageDiffGrid(MarketDataSplitter* parent = nullptr, DataFile* dataFile = nullptr);
    void initial();
    void drawVolume();
    void virtual paintEvent(QPaintEvent* event) override;
    void virtual keyPressEvent(QKeyEvent* event) override;
    void virtual mouseMoveEvent(QMouseEvent* event) override;
    void virtual mousePressEvent(QMouseEvent* event) override;
    void virtual mouseReleaseEvent(QMouseEvent* event) override;
    void getIndicator();

private:
    void drawCross2();
    void drawCross();
    void drawMouseMoveCrossHorLine();
    void drawMouseMoveCrossVerLine();
    void drawCrossVerLine();
    void drawCrossHorLine();
    void drawTopInfo();

private:

    //当前要画的成交量线中的最大成交量
    double maxVolume;

    int ypZeroLine;

    //线宽
    int lineWidth;

    int topInfoHeight = 20;
};

#endif // CAPITALAVERAGEDIFF_H
