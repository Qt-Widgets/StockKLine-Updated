#ifndef TOPINSTRUMENTSUMMARY_H
#define TOPINSTRUMENTSUMMARY_H

#include <QWidget>
#include "autogrid.h"

class TopInstrumentSummary : public AutoGrid
{
    Q_OBJECT

public:
    TopInstrumentSummary(QWidget* parent);
    void virtual paintEvent(QPaintEvent* event);
};

#endif // TOPINSTRUMENTSUMMARY_H
