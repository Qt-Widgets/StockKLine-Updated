#include <QSplitter>
#include "kvolumegrid.h"
#include "marketdatatab.h"
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    tabWidget = new QTabWidget(this);
    tabWidget->addTab(new MarketDataTab(), QStringLiteral(""));

//    pgrid = new kVolumeGrid(this);
//    pgrid->setObjectName(QStringLiteral("kline"));
//    pgrid->setFocusPolicy(Qt::StrongFocus);

//    auto p2 = new KLineGrid(this);
//    p2->setFocusPolicy(Qt::StrongFocus);

//   // setCentralWidget(pgrid);



//    QSplitter *splitterMain = new QSplitter(Qt::Vertical, nullptr); //新建主分割窗口，水平分割
//    QSplitter *splitterLeft = new QSplitter(Qt::Vertical, splitterMain);
//    QSplitter *splitterRight = new QSplitter(Qt::Vertical, splitterMain);


//    splitterMain->setHandleWidth(1);

//    splitterLeft->addWidget(p2);
//    splitterRight->addWidget(pgrid);
//    this->setCentralWidget(splitterMain);


    resize(1600,800);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete tabWidget;
}
