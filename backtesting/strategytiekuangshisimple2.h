#ifndef STRATEGYTIEKUANGSHISIMPLE2_H
#define STRATEGYTIEKUANGSHISIMPLE2_H

#include <string>
#include <vector>
#include "Strategy.h"
#include "MA.h"
#include "backtesting/backtestingconfig.h"
#include "WindowedMaxMin.h"

class StrategyTieKuangShiSimple2 : public Strategy
{
public:
    StrategyTieKuangShiSimple2();
    virtual ~StrategyTieKuangShiSimple2() {}

    virtual void init(TradeGatewayPtr pTradeGateway) override;
    virtual void onTick(Tick &tick) override;
    virtual void onBar(KLineDataType &bar) override;
    virtual void onTrade(Trade &trade) override;
    virtual void onOrder(OrderRsp &order) override;
    virtual std::string name() override;
    virtual std::string instrumentType() override;

private:
    int getCloseMinute(KLineDataType &bar);
    void closePosition(int volume);
    void closeAllPosition();
    void openPosition(int volume);
    bool crossDK();
    bool crossKD();

    void BPK(int volume);
    void SPK(int volume);
    void SP(int volume);
    void BP(int volume);
    void SK(int volume);
    void BK(int volume);

private:
    TradeGatewayPtr tradeGatewayPtr_;

    VecPtr closeVecPtr_;

    double currentDiff_, lastDiff_;

    MA maClose5Tracker;
    MA maClose6Tracker;
    MA maClose20Tracker;
    MA maClose250Tracker;

    BacktestingConfig* backtestingConfig;

    double lastK_;
    double currentK_;
    double lastD_;
    double currentD_;
    double lastRsv_;
    WindowedMaxMin windowedMaxMin_;
};

#endif // STRATEGYTIEKUANGSHISIMPLE_H
