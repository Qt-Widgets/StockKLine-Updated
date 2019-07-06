#include "datadetailbox.h"
#include <QStyleOption>
#include <QPainter>

DataDetailBox::DataDetailBox(QWidget* parent)
    : QWidget(parent)
{
    this->setContentsMargins(0, marginTop, 0, 0);
    this->setStyleSheet("background-color: black");
    this->setStyleSheet("border: 1px solid white");
    this->setFixedSize(width, height);
}

void DataDetailBox::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
