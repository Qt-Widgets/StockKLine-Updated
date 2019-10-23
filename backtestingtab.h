#ifndef BACKTESTING_H
#define BACKTESTING_H

#include <QWidget>
#include "datafile.h"
#include "backtesting/backtestingconfig.h"

class BacktestingTab : public QWidget
{
    Q_OBJECT

public:
    explicit BacktestingTab(QWidget *parent = nullptr);
    ~BacktestingTab();

    void loadData();

private:
    QWidget* createChartWidget(QWidget* parent);
    QWidget* createAdminWidget(QWidget* parent);

private:
    DataFile* mDataFile;
    BacktestingConfig* backtestingConfig;
};

#endif // BACKTESTING_H
