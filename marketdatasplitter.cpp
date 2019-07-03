#include "marketdatasplitter.h"

void MarketDataSplitter::childKeyPressEvent(QKeyEvent* event)
{
    emit childKeyPressed(event);
}

void MarketDataSplitter::childMouseMoveEvent(QMouseEvent* event)
{
    emit childMouseMoved(event);
}

void MarketDataSplitter::childMousePressEvent(QMouseEvent* event)
{
    emit childMousePressed(event);
}
