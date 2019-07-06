#include "datawidget.h"

DataWidget::DataWidget(MarketDataSplitter* parent, DataFile* dataFile, bool needGrid)
    : AutoGrid(parent, needGrid)
{
    this->mDataFile = dataFile;

    connect(parent, &MarketDataSplitter::childMouseMoved, this, &DataWidget::mouseMoveEventFromParent);
    connect(parent, &MarketDataSplitter::childMousePressed, this, &DataWidget::mousePressEventFromParent);
    connect(parent, &MarketDataSplitter::childKeyPressed, this, &DataWidget::keyPressEventFromParent);
}

void DataWidget::keyPressEventFromParent(QKeyEvent* event)
{

}
void DataWidget::mouseMoveEventFromParent(QMouseEvent* event)
{

}
void DataWidget::mousePressEventFromParent(QMouseEvent* event)
{

}
