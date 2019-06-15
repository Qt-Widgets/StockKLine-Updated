#ifndef BACKTESTING_H
#define BACKTESTING_H

#include <QWidget>

class Backtesting : public QWidget
{
    Q_OBJECT

public:
    explicit Backtesting(QWidget *parent = nullptr);
};

#endif // BACKTESTING_H
