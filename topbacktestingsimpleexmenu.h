#ifndef TOPBACKTESTINGSIMPLEEXMENU_H
#define TOPBACKTESTINGSIMPLEEXMENU_H

#include <QLineEdit>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
#include "autogrid.h"
#include "backtesting/backtestingconfig.h"

class TopBacktestingSimpleExMenu : public AutoGrid
{
    Q_OBJECT

public:
    TopBacktestingSimpleExMenu(QWidget* parent);
    ~TopBacktestingSimpleExMenu();
    void virtual paintEvent(QPaintEvent* event);

    QLineEdit *getAvgIntervalEdit() const;

    QLineEdit *getBaseLotEdit() const;

    QLineEdit *getCapitalPeriodEdit() const;

    QLineEdit *getAddLotDiffThreshold1Edit() const;

    QLineEdit *getAddLotBacktrackThreshold1Edit() const;

    QLineEdit *getAddLotDiffThreshold2Edit() const;

    QLineEdit *getAddLotBacktrackThreshold2Edit() const;

    QLineEdit *getDecLotDiffThreshold1Edit() const;

    QLineEdit *getDecLotDiffThreshold2Edit() const;

    QPushButton *getRunTestButton() const;

private:
    void drawBottomLine();
    QLineEdit* appendLabelEdit(QHBoxLayout* hbox, QString label, QPalette& palette);
    QLineEdit* appendEdit(QHBoxLayout* hbox, QPalette& palette);

private:
    int barHeight = 20;
    QLineEdit* avgIntervalEdit;
    QLineEdit* baseLotEdit;
    QLineEdit* capitalPeriodEdit;

    QLineEdit* addLotDiffThreshold1Edit;
    QLineEdit* addLotBacktrackThreshold1Edit;
    QLineEdit* addLotDiffThreshold2Edit;
    QLineEdit* addLotBacktrackThreshold2Edit;

    QLineEdit* decLotDiffThreshold1Edit;
    QLineEdit* decLotDiffThreshold2Edit;

    QPushButton* runTestButton;

    BacktestingConfig* backTestConfig;
};

#endif // TOPBACKTESTINGSIMPLEEXMENU_H
