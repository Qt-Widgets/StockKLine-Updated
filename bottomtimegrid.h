#ifndef BOTTOMTIMEBAR_H
#define BOTTOMTIMEBAR_H

#include "datawidget.h"
#include "marketdatasplitter.h"

class BottomTimeGrid : public DataWidget
{
    Q_OBJECT

public:
    explicit BottomTimeGrid(MarketDataSplitter* parent = nullptr, DataFile* dataFile = nullptr);
    void paintEvent(QPaintEvent* event) override;

private:
    int tipsHeight = 20;
    int tipsWidth = 120;
};

#endif // BOTTOMTIMEBAR_H
