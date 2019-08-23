#ifndef BACKTESTING_H
#define BACKTESTING_H

#include <QWidget>
#include "datafile.h"

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
};

#endif // BACKTESTING_H
