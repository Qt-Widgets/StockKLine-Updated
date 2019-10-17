#ifndef STRATEGYJIAOTANSIMPLE2_H
#define STRATEGYJIAOTANSIMPLE2_H

#include <string>
#include <vector>
#include "Strategy.h"
#include "MA.h"
#include "backtesting/backtestingconfig.h"

class StrategyJiaoTanSimple2 : public Strategy
{
public:
    StrategyJiaoTanSimple2();
    virtual ~StrategyJiaoTanSimple2() {}

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
    double currentDiff1_, lastDiff1_;
    double currentMA15_, lastMA15_;
    double currentMA50_, lastMA50_;

    MA maClose10Tracker;
    MA maClose17Tracker;
    MA maClose15Tracker;
    MA maClose50Tracker;
    MA maClose170Tracker;
    MA maClose190Tracker;

    BacktestingConfig* backtestingConfig;
};

#endif // STRATEGYJIAOTANSIMPLE2_H
