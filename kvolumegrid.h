#ifndef KVOLUMEGRID_H
#define KVOLUMEGRID_H

#include "datawidget.h"
#include "datafile.h"
#include "marketdatasplitter.h"

#include <QPoint>

class kVolumeGrid : public DataWidget
{
public:
    explicit kVolumeGrid(MarketDataSplitter* parent = nullptr, DataFile* dataFile = nullptr);
    void initial();
    void drawYtick();
    void drawVolume();
    void virtual paintEvent(QPaintEvent* event) override;
    void virtual keyPressEvent(QKeyEvent* event) override;
    void virtual mouseMoveEvent(QMouseEvent* event) override;
    void virtual mousePressEvent(QMouseEvent* event) override;
    void virtual mouseReleaseEvent(QMouseEvent* event) override;
    void getIndicator();
    void drawAverageLine(int day);

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

    //线宽
    int lineWidth;

    int topInfoHeight = 20;
};

#endif // KVOLUMEGRID_H
