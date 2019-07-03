#include <QVBoxLayout>
#include "kvolumegrid.h"
#include "klinegrid.h"
#include "marketdatatab.h"
#include "marketdataadmin.h"
#include "marketdatasplitter.h"

MarketDataTab::MarketDataTab(QWidget *parent)
    : QWidget(parent)
{
//    QSplitter *splitter = new QSplitter(Qt::Horizontal, nullptr);
//    //splitter->addWidget(createAdminWidget(splitter));
//    //splitter->addWidget(createChartWidget(splitter));
//    createAdminWidget(splitter);
//    createChartWidget(splitter);
//    splitter->setHandleWidth(1);
//    splitter->setStretchFactor(0, 1);
//    splitter->setStretchFactor(1, 4);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(createChartWidget(nullptr));
    this->setLayout(layout);
}

QWidget* MarketDataTab::createChartWidget(QWidget* parent)
{
    MarketDataSplitter *splitterMain = new MarketDataSplitter(parent); //新建主分割窗口，水平分割

    auto p2 = new KLineGrid(splitterMain);
    p2->setFocusPolicy(Qt::StrongFocus);

    auto pgrid = new kVolumeGrid(splitterMain);
    pgrid->setObjectName(QStringLiteral("kline"));
    pgrid->setFocusPolicy(Qt::StrongFocus);

    splitterMain->setHandleWidth(1);
    splitterMain->setStretchFactor(0, 2);
    splitterMain->setStretchFactor(1, 1);
    return splitterMain;
}

QWidget* MarketDataTab::createAdminWidget(QWidget* parent)
{
    return new MarketDataAdmin(parent);
}
