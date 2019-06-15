#include <QVBoxLayout>
#include "marketdataadmin.h"

MarketDataAdmin::MarketDataAdmin(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(parent);
    this->setLayout(layout);
}
