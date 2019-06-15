#ifndef MARKETDATATAB_H
#define MARKETDATATAB_H

#include <QWidget>

class MarketDataTab : public QWidget
{
    Q_OBJECT

public:
    explicit MarketDataTab(QWidget *parent = nullptr);

private:
    QWidget* createChartWidget(QWidget* parent);
    QWidget* createAdminWidget(QWidget* parent);
};

#endif // MARKETDATATAB_H
