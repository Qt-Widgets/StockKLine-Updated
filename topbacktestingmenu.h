#ifndef TOPBACKTESTINGMENU_H
#define TOPBACKTESTINGMENU_H

#include <QLineEdit>
#include <QHBoxLayout>
#include <QCheckBox>
#include <QPushButton>
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

    QLineEdit *getPosLotThreshold1Edit() const;

    QLineEdit *getPosLotThreshold2Edit() const;

    QLineEdit *getPosLotThreshold3Edit() const;

    QLineEdit *getPosLotThreshold4Edit() const;

    QLineEdit *getPosLotThreshold5Edit() const;

    QLineEdit *getNegLotThreshold1Edit() const;

    QLineEdit *getNegLotThreshold2Edit() const;

    QLineEdit *getNegLotThreshold3Edit() const;

    QLineEdit *getBaseLotEdit() const;

    QCheckBox *getEnableCapitalAjdustmentCheckBox() const;

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

    QLineEdit* posThreshold1Edit;
    QLineEdit* posThreshold2Edit;
    QLineEdit* posThreshold3Edit;
    QLineEdit* posThreshold4Edit;
    QLineEdit* posThreshold5Edit;
    QLineEdit* negThreshold1Edit;
    QLineEdit* negThreshold2Edit;
    QLineEdit* negThreshold3Edit;

    QLineEdit* posLotThreshold1Edit;
    QLineEdit* posLotThreshold2Edit;
    QLineEdit* posLotThreshold3Edit;
    QLineEdit* posLotThreshold4Edit;
    QLineEdit* posLotThreshold5Edit;
    QLineEdit* negLotThreshold1Edit;
    QLineEdit* negLotThreshold2Edit;
    QLineEdit* negLotThreshold3Edit;

    QCheckBox* enableCapitalAjdustmentCheckBox;
    QPushButton* runTestButton;

    BacktestingConfig* backTestConfig;
};

#endif // TOPBACKTESTINGMENU_H
