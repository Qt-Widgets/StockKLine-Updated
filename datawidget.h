#ifndef DATAWIDGET_H
#define DATAWIDGET_H

#include <QMouseEvent>
#include <QKeyEvent>
#include "autogrid.h"
#include "datafile.h"
#include "marketdatasplitter.h"

class DataWidget : public AutoGrid
{
    Q_OBJECT

public:
    DataWidget(MarketDataSplitter* parent = nullptr, DataFile* dataFile = nullptr, bool needGrid = true);
    //virtual ~DataWiddget();

public slots:
    void keyPressEventFromParent(QKeyEvent* event);
    void mouseMoveEventFromParent(QMouseEvent* event);
    void mousePressEventFromParent(QMouseEvent* event);

protected:
    DataFile* mDataFile;

    //画k线的起始日期和终止日期
    int beginDay;
    int endDay;
    int totalDay;
    int currentDay;

    //鼠标位置
    QPoint mousePoint;
    bool isUnderMouse = false;

    //键盘是否按下
    bool isKeyDown = false;
};

#endif // DATAWIDGET_H
