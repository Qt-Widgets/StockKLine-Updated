#ifndef MARKETDATATAB_H
#define MARKETDATATAB_H

#include <QWidget>
#include "datafile.h"
#include "backtesting/backtestingconfig.h"

class MarketDataTab : public QWidget
{
    Q_OBJECT

public:
    explicit MarketDataTab(QWidget *parent = nullptr);
    ~MarketDataTab();
    void loadData();

private:
    QWidget* createChartWidget(QWidget* parent);
    QWidget* createAdminWidget(QWidget* parent);

private:
    DataFile* mDataFile;
    BacktestingConfig* backtestingConfig;
};

#endif // MARKETDATATAB_H
