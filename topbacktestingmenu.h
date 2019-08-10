#ifndef TOPBACKTESTINGMENU_H
#define TOPBACKTESTINGMENU_H

#include <QLineEdit>
#include "autogrid.h"

class TopBacktestingMenu : public AutoGrid
{
    Q_OBJECT

public:
    TopBacktestingMenu(QWidget* parent);
    ~TopBacktestingMenu();
    void virtual paintEvent(QPaintEvent* event);

    QLineEdit* getAvgIntervalEdit();

private:
    void drawBottomLine();

private:
    int barHeight = 20;
    QLineEdit* avgIntervalEdit;
};

#endif // TOPBACKTESTINGMENU_H
