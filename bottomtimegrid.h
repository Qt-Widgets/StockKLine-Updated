#ifndef BOTTOMTIMEBAR_H
#define BOTTOMTIMEBAR_H

#include "datawidget.h"
#include "marketdatasplitter.h"

class BottomTimeGrid : public DataWidget
{
    Q_OBJECT

public:
    explicit BottomTimeGrid(MarketDataSplitter* parent = nullptr, DataFile* dataFile = nullptr);
    void virtual paintEvent(QPaintEvent* event);

private:
    void mouseMoveEventFromParent(QMouseEvent* event);
    void mousePressEventFromParent(QMouseEvent* event);
    void keyPressEventFromParent(QKeyEvent* event);
};

#endif // BOTTOMTIMEBAR_H
