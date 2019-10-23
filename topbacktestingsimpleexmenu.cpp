#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include "topbacktestingsimpleexmenu.h"

TopBacktestingSimpleExMenu::TopBacktestingSimpleExMenu(QWidget* parent)
    : backTestConfig(BacktestingConfig::instance()), AutoGrid (parent, false)
{
    this->setFixedHeight(barHeight);

    QFont ft;
    ft.setPointSize(10);
    this->setFont(ft);

    QHBoxLayout* hbox = new QHBoxLayout(this);
    hbox->setContentsMargins(50, 0, 0, 0);
    hbox->setSpacing(1);

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);

    QLabel* productLabel = new QLabel(QStringLiteral("产品: "));
    productLabel->setPalette(palette);
    productLabel->setMaximumWidth(50);
    productLabel->setMinimumWidth(50);
    hbox->addWidget(productLabel);

    productSelect = new QComboBox();
    hbox->addWidget(productSelect);
    productSelect->addItem(QStringLiteral("铁矿石"));
    productSelect->addItem(QStringLiteral("焦炭"));
    productSelect->addItem(QStringLiteral("螺纹钢"));

    QLabel* avgIntervalLabel = new QLabel(QStringLiteral("   图中均线: "));
    avgIntervalLabel->setPalette(palette);
    hbox->addWidget(avgIntervalLabel);

    avgIntervalEdit = new QLineEdit();
    avgIntervalEdit->setStyleSheet("border: 1px solid red; background-color: black;");
    avgIntervalEdit->setMaximumWidth(50);
    avgIntervalEdit->setMaximumHeight(barHeight - 4);
    avgIntervalEdit->setText(QString::number(250));
    palette.setColor(QPalette::Text, Qt::white);
    avgIntervalEdit->setPalette(palette);
    hbox->addWidget(avgIntervalEdit);

    capitalPeriodEdit = appendLabelEdit(hbox, QStringLiteral("回测均线: "), palette);
    capitalPeriodEdit->setText(QString::number(backTestConfig->capitalPeriod));

    baseLotEdit = appendLabelEdit(hbox, QStringLiteral("基础单: "), palette);
    baseLotEdit->setText(QString::number(backTestConfig->baseLot));

    tieKuangShiNEdit = appendLabelEdit(hbox, QStringLiteral("N: "), palette);
    tieKuangShiNEdit->setText(QString::number(backTestConfig->tieKuangShiN));

//    addLotDiffThreshold2Edit = appendLabelEdit(hbox, QStringLiteral("加仓偏离值2(%%): "), palette);
//    addLotDiffThreshold2Edit->setText(QString::number(backTestConfig->addLotDiffThreshold2));
//    addLotBacktrackThreshold2Edit = appendLabelEdit(hbox, QStringLiteral("加仓回撤值2: "), palette);
//    addLotBacktrackThreshold2Edit->setText(QString::number(backTestConfig->addLotBacktrackThreshold2));

    addLotDiffThreshold1Edit = appendLabelEdit(hbox, QStringLiteral("加仓偏离值: "), palette);
    addLotDiffThreshold1Edit->setText(QString::number(backTestConfig->addLotDiffThreshold1));
    addLotBacktrackThreshold1Edit = appendLabelEdit(hbox, QStringLiteral("加仓回撤值: "), palette);
    addLotBacktrackThreshold1Edit->setText(QString::number(backTestConfig->addLotBacktrackThreshold1));

    decLotDiffThreshold1Edit = appendLabelEdit(hbox, QStringLiteral("中位减仓偏离值: "), palette);
    decLotDiffThreshold1Edit->setText(QString::number(backTestConfig->decLotDiffThreshold1));

    decLotDiffThreshold2Edit = appendLabelEdit(hbox, QStringLiteral("高位清仓偏离值: "), palette);
    decLotDiffThreshold2Edit->setText(QString::number(backTestConfig->decLotDiffThreshold2));

    runTestButton = new QPushButton(QStringLiteral("运行回测"));
    hbox->addWidget(runTestButton);

    hbox->addStretch(1);
    this->setLayout(hbox);
}

QLineEdit* TopBacktestingSimpleExMenu::appendLabelEdit(QHBoxLayout* hbox, QString labelString, QPalette& palette)
{
    QLabel* label = new QLabel(labelString);
    label->setPalette(palette);
    hbox->addWidget(label);

    QLineEdit* edit = appendEdit(hbox, palette);

    return edit;
}

QLineEdit* TopBacktestingSimpleExMenu::appendEdit(QHBoxLayout* hbox, QPalette& palette)
{
    QLineEdit* edit = new QLineEdit();
    edit->setStyleSheet("border: 1px solid red; background-color: black;");
    edit->setMaximumWidth(50);
    edit->setMaximumHeight(barHeight - 4);
    palette.setColor(QPalette::Text, Qt::white);
    edit->setPalette(palette);
    hbox->addWidget(edit);

    return edit;
}

QComboBox *TopBacktestingSimpleExMenu::getProductSelect() const
{
    return productSelect;
}

QLineEdit *TopBacktestingSimpleExMenu::getTieKuangShiNEdit() const
{
    return tieKuangShiNEdit;
}

QPushButton *TopBacktestingSimpleExMenu::getRunTestButton() const
{
    return runTestButton;
}

QLineEdit *TopBacktestingSimpleExMenu::getDecLotDiffThreshold2Edit() const
{
    return decLotDiffThreshold2Edit;
}

QLineEdit *TopBacktestingSimpleExMenu::getDecLotDiffThreshold1Edit() const
{
    return decLotDiffThreshold1Edit;
}

QLineEdit *TopBacktestingSimpleExMenu::getAddLotBacktrackThreshold2Edit() const
{
    return addLotBacktrackThreshold2Edit;
}

QLineEdit *TopBacktestingSimpleExMenu::getAddLotDiffThreshold2Edit() const
{
    return addLotDiffThreshold2Edit;
}

QLineEdit *TopBacktestingSimpleExMenu::getAddLotBacktrackThreshold1Edit() const
{
    return addLotBacktrackThreshold1Edit;
}

QLineEdit *TopBacktestingSimpleExMenu::getAddLotDiffThreshold1Edit() const
{
    return addLotDiffThreshold1Edit;
}

QLineEdit *TopBacktestingSimpleExMenu::getCapitalPeriodEdit() const
{
    return capitalPeriodEdit;
}

QLineEdit *TopBacktestingSimpleExMenu::getBaseLotEdit() const
{
    return baseLotEdit;
}

QLineEdit *TopBacktestingSimpleExMenu::getAvgIntervalEdit() const
{
    return avgIntervalEdit;
}

TopBacktestingSimpleExMenu::~TopBacktestingSimpleExMenu()
{
    if (!avgIntervalEdit) {
        delete avgIntervalEdit;
    }
}

void TopBacktestingSimpleExMenu::paintEvent(QPaintEvent* event)
{
    drawBottomLine();
}

void TopBacktestingSimpleExMenu::drawBottomLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawLine(0, barHeight - 1, getGridWidth() + getMarginLeft() + getMarginRight(), barHeight - 1);
}
