#include <QVBoxLayout>
#include <QMessageBox>
#include "capitalaveragediffgrid.h"
#include "capitallinegrid.h"
#include "backtestingtab.h"
#include "marketdataadmin.h"
#include "marketdatasplitter.h"
#include "datadetailbox.h"
#include "bottomtimegrid.h"
#include "topbacktestingmenu.h"

BacktestingTab::BacktestingTab(QWidget *parent)
    : QWidget(parent)
{
    loadData();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(createChartWidget(nullptr));
    this->setLayout(layout);
}

BacktestingTab::~BacktestingTab()
{
    delete mDataFile;
}

void BacktestingTab::loadData()
{
    mDataFile = new DataFile();

    //读取数据
    //QString file = QStringLiteral("F:\\qt-projects\\StockKLine\\dataKLine.txt");

    QString file = QStringLiteral("E:\\cbm\\startup\\qihuoshuju_good\\TieKuangShi_15min_I.csv");
    if( !mDataFile->readData(file) )
    {
        QMessageBox::about(this, QStringLiteral("数据文件读取失败"), QStringLiteral("确定"));
        return;
    }

    file = QStringLiteral("E:\\cbm\\startup\\qihuoshuju_good\\TieKuangshi_15min_Backtesting_Stats.csv");
    if( !mDataFile->readBacktestingResult(file) )
    {
        QMessageBox::about(this, QStringLiteral("数据文件读取失败"), QStringLiteral("确定"));
        return;
    }
}

QWidget* BacktestingTab::createChartWidget(QWidget* parent)
{
    MarketDataSplitter *splitterMain = new MarketDataSplitter(parent); //新建主分割窗口，水平分割

    auto topInfo = new TopBacktestingMenu(splitterMain);
    topInfo->setFocusPolicy(Qt::StrongFocus);

    auto kline = new CapitalLineGrid(splitterMain, mDataFile);
    kline->setFocusPolicy(Qt::StrongFocus);
    kline->trackTopBacktestingMenu(topInfo);

    auto volume = new CapitalAverageDiffGrid(splitterMain, mDataFile);
    volume->setObjectName(QStringLiteral("kline"));
    volume->setFocusPolicy(Qt::StrongFocus);

    auto timeGrid = new BottomTimeGrid(splitterMain, mDataFile);
    timeGrid->setFocusPolicy(Qt::StrongFocus);

    splitterMain->setHandleWidth(1);
    splitterMain->setStyleSheet("QSplitter::handle { background-color: #000000; }");
    splitterMain->setStretchFactor(1, 2);
    splitterMain->setStretchFactor(2, 1);

    return splitterMain;
}

QWidget* BacktestingTab::createAdminWidget(QWidget* parent)
{
    return new MarketDataAdmin(parent);
}
