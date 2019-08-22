#ifndef TOPBACKTESTINGMENU_H
#define TOPBACKTESTINGMENU_H

#include <QLineEdit>
#include <QHBoxLayout>
#include "autogrid.h"
#include "backtesting/backtestingconfig.h"

class TopBacktestingMenu : public AutoGrid
{
    Q_OBJECT

public:
    TopBacktestingMenu(QWidget* parent);
    ~TopBacktestingMenu();
    void virtual paintEvent(QPaintEvent* event);

    QLineEdit* getAvgIntervalEdit();

    QLineEdit *getCapitalPeriodEdit() const;

    QLineEdit *getPosThreshold1Edit() const;

    QLineEdit *getPosThreshold2Edit() const;

    QLineEdit *getPosThreshold3Edit() const;

    QLineEdit *getPosThreshold4Edit() const;

    QLineEdit *getPosThreshold5Edit() const;

    QLineEdit *getNegThreshold1Edit() const;

    QLineEdit *getNegThreshold2Edit() const;

    QLineEdit *getNegThreshold3Edit() const;

private:
    void drawBottomLine();
    QLineEdit* appendEdit(QHBoxLayout* hbox, QString label);

private:
    int barHeight = 20;
    QLineEdit* avgIntervalEdit;
    QLineEdit* capitalPeriodEdit;
    QLineEdit* posThreshold1Edit;
    QLineEdit* posThreshold2Edit;
    QLineEdit* posThreshold3Edit;
    QLineEdit* posThreshold4Edit;
    QLineEdit* posThreshold5Edit;
    QLineEdit* negThreshold1Edit;
    QLineEdit* negThreshold2Edit;
    QLineEdit* negThreshold3Edit;

    BacktestingConfig* backTestConfig;
};

#endif // TOPBACKTESTINGMENU_H
