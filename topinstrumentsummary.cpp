#include <QPainter>
#include "topinstrumentsummary.h"

TopInstrumentSummary::TopInstrumentSummary(QWidget* parent)
    : AutoGrid (parent, false)
{
    this->setFixedHeight(20);
}

void TopInstrumentSummary::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    QPen     pen;
    pen.setColor(QColor("#FF0000"));
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);

    painter.drawLine(0, 20 -1, getGridWidth() + getMarginLeft() + getMarginRight(), 20-1);
}
