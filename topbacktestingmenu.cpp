#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include "topbacktestingmenu.h"

TopBacktestingMenu::TopBacktestingMenu(QWidget* parent)
    : backTestConfig(BacktestingConfig::instance()), AutoGrid (parent, false)
{
    this->setFixedHeight(barHeight);

    QFont ft;
    ft.setPointSize(10);
    this->setFont(ft);

    QHBoxLayout* hbox = new QHBoxLayout();
    hbox->setContentsMargins(50, 0, 0, 0);
    hbox->setSpacing(1);

    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);

    QLabel* productLabel = new QLabel(QStringLiteral("产品: "));
    productLabel->setPalette(palette);
    productLabel->setMaximumWidth(100);
    productLabel->setMinimumWidth(100);
    hbox->addWidget(productLabel);

    QLabel* avgIntervalLabel = new QLabel(QStringLiteral("图中均线周期: "));
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

    capitalPeriodEdit = appendEdit(hbox, QStringLiteral("回测均线周期: "));
    capitalPeriodEdit->setText(QString::number(backTestConfig->capitalPeriod));
    negThreshold3Edit = appendEdit(hbox, QStringLiteral("负值3: "));
    negThreshold3Edit->setText(QString::number(backTestConfig->negThreshold3));
    negThreshold2Edit = appendEdit(hbox, QStringLiteral("负值2: "));
    negThreshold2Edit->setText(QString::number(backTestConfig->negThreshold2));
    negThreshold1Edit = appendEdit(hbox, QStringLiteral("负值1: "));
    negThreshold1Edit->setText(QString::number(backTestConfig->negThreshold1));
    posThreshold1Edit = appendEdit(hbox, QStringLiteral("正值1: "));
    posThreshold1Edit->setText(QString::number(backTestConfig->posThreshold1));
    posThreshold2Edit = appendEdit(hbox, QStringLiteral("正值2: "));
    posThreshold2Edit->setText(QString::number(backTestConfig->posThreshold2));
    posThreshold3Edit = appendEdit(hbox, QStringLiteral("正值3: "));
    posThreshold3Edit->setText(QString::number(backTestConfig->posThreshold3));
    posThreshold4Edit = appendEdit(hbox, QStringLiteral("正值4: "));
    posThreshold4Edit->setText(QString::number(backTestConfig->posThreshold4));
    posThreshold5Edit = appendEdit(hbox, QStringLiteral("正值5: "));
    posThreshold5Edit->setText(QString::number(backTestConfig->posThreshold5));

    hbox->addStretch(1);
    this->setLayout(hbox);
}

QLineEdit* TopBacktestingMenu::appendEdit(QHBoxLayout* hbox, QString labelString)
{
    QPalette palette;
    palette.setColor(QPalette::WindowText, Qt::white);

    QLabel* label = new QLabel(labelString);
    label->setPalette(palette);
    hbox->addWidget(label);

    QLineEdit* edit = new QLineEdit();
    edit->setStyleSheet("border: 1px solid red; background-color: black;");
    edit->setMaximumWidth(50);
    edit->setMaximumHeight(barHeight - 4);
    palette.setColor(QPalette::Text, Qt::white);
    edit->setPalette(palette);
    hbox->addWidget(edit);

    return edit;
}

QLineEdit *TopBacktestingMenu::getNegThreshold3Edit() const
{
    return negThreshold3Edit;
}

QLineEdit *TopBacktestingMenu::getNegThreshold2Edit() const
{
    return negThreshold2Edit;
}

QLineEdit *TopBacktestingMenu::getNegThreshold1Edit() const
{
    return negThreshold1Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold5Edit() const
{
    return posThreshold5Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold4Edit() const
{
    return posThreshold4Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold3Edit() const
{
    return posThreshold3Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold2Edit() const
{
    return posThreshold2Edit;
}

QLineEdit *TopBacktestingMenu::getPosThreshold1Edit() const
{
    return posThreshold1Edit;
}

QLineEdit *TopBacktestingMenu::getCapitalPeriodEdit() const
{
    return capitalPeriodEdit;
}

QLineEdit* TopBacktestingMenu::getAvgIntervalEdit()
{
    return avgIntervalEdit;
}

TopBacktestingMenu::~TopBacktestingMenu()
{
    if (!avgIntervalEdit) {
        delete avgIntervalEdit;
    }
}

void TopBacktestingMenu::paintEvent(QPaintEvent* event)
{
    drawBottomLine();
}

void TopBacktestingMenu::drawBottomLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawLine(0, barHeight - 1, getGridWidth() + getMarginLeft() + getMarginRight(), barHeight - 1);
}
