#include <QPainter>
#include "topinstrumentsummary.h"

TopInstrumentSummary::TopInstrumentSummary(QWidget* parent)
    : AutoGrid (parent, false)
{
    this->setFixedHeight(barHeight);

    QFont ft;
    ft.setPointSize(10);
    this->setFont(ft);
}

void TopInstrumentSummary::paintEvent(QPaintEvent* event)
{
    drawBottomLine();

    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor("#FFFFFF"));
    painter.setPen(pen);

    QRect rect(30, 2, 100, barHeight - 3);
    QString str = QStringLiteral("产品");
    painter.drawText(rect,str);
}

void TopInstrumentSummary::drawBottomLine()
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawLine(0, barHeight - 1, getGridWidth() + getMarginLeft() + getMarginRight(), barHeight - 1);
}
