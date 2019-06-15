#include <QSplitter>
#include "kvolumegrid.h"
#include "klinegrid.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "marketdatatab.h"
#include "backtesting.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);

    tabWidget = new QTabWidget();
    tabWidget->addTab(new MarketDataTab(), QString("市场数据"));
    tabWidget->addTab(new Backtesting(), QString("历史回测"));
    this->setCentralWidget(tabWidget);

    resize(1600,800);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete tabWidget;
}
