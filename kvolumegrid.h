#ifndef KVOLUMEGRID_H
#define KVOLUMEGRID_H


#include "autogrid.h"
#include "datafile.h"
#include "marketdatasplitter.h"

#include <QPoint>

class kVolumeGrid : public AutoGrid
{
public:
    explicit kVolumeGrid(MarketDataSplitter* parent);
    bool readData(QString strFile);
    void initial();
    void drawYtick();
    void drawVolume();
    void virtual paintEvent(QPaintEvent* event);
    void virtual resizeEvent(QResizeEvent* event);
    void virtual keyPressEvent(QKeyEvent* event);
    void virtual mouseMoveEvent(QMouseEvent* event);
    void virtual mousePressEvent(QMouseEvent* event);
    void getIndicator();
    void drawAverageLine(int day);

public slots:
    void keyPressEventFromParent(QKeyEvent* event);
    void mouseMoveEventFromParent(QMouseEvent* event);
    void mousePressEventFromParent(QMouseEvent* event);

private:
    void drawCross2();
    void drawCross();
    void drawMouseMoveCrossHorLine();
    void drawMouseMoveCrossVerLine();
    void drawCrossVerLine();
    void drawCrossHorLine();

private:
    DataFile mDataFile;

    //画成交量线的起始日期和终止日期
    int beginDay;
    int endDay;
    int totalDay;
    int currentDay;



    //当前要画的成交量线中的最大成交量
    double maxVolume;

    //是否显示十字线
    bool bCross = false;


    //鼠标位置
    QPoint mousePoint;
    bool isUnderMouse = false;

    //键盘是否按下
    bool isKeyDown = false;


    //线宽
    int lineWidth;


};

#endif // KVOLUMEGRID_H
