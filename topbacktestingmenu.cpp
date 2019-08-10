#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include "topbacktestingmenu.h"

TopBacktestingMenu::TopBacktestingMenu(QWidget* parent)
    : AutoGrid (parent, false)
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

    QLabel* avgIntervalLabel = new QLabel(QStringLiteral("均线周期: "));
    avgIntervalLabel->setPalette(palette);
    hbox->addWidget(avgIntervalLabel);

    avgIntervalEdit = new QLineEdit();
    avgIntervalEdit->setStyleSheet("border: 1px solid red; background-color: black;");
    avgIntervalEdit->setMaximumWidth(50);
    avgIntervalEdit->setMaximumHeight(barHeight - 4);
    palette.setColor(QPalette::Text, Qt::white);
    avgIntervalEdit->setPalette(palette);
    hbox->addWidget(avgIntervalEdit);

    hbox->addStretch(1);
    this->setLayout(hbox);
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
