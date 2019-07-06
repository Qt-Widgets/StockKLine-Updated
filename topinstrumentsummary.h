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

private:
    void drawBottomLine();

private:
    int barHeight = 20;
};

#endif // TOPINSTRUMENTSUMMARY_H
