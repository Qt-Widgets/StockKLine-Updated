#ifndef MARKETDATATAB_H
#define MARKETDATATAB_H

#include <QWidget>
#include "datafile.h"

class MarketDataTab : public QWidget
{
    Q_OBJECT

public:
    explicit MarketDataTab(QWidget *parent = nullptr);
    ~MarketDataTab();

private:
    QWidget* createChartWidget(QWidget* parent);
    QWidget* createAdminWidget(QWidget* parent);
    void loadData();

private:
    DataFile* mDataFile;
};

#endif // MARKETDATATAB_H
