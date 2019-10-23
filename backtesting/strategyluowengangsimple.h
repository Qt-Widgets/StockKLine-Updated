#ifndef STRATEGYLUOWENGANGSIMPLE_H
#define STRATEGYLUOWENGANGSIMPLE_H

#include <string>
#include <vector>
#include "Strategy.h"
#include "MA.h"
#include "backtesting/backtestingconfig.h"

class StrategyLuoWenGangSimple : public Strategy
{
public:
    StrategyLuoWenGangSimple();
    virtual ~StrategyLuoWenGangSimple() {}

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
    double currentMA12_, lastMA12_;
    double currentMA250_, lastMA250_;

    MA maClose5Tracker;
    MA maClose12Tracker;
    MA maClose250Tracker;

    BacktestingConfig* backtestingConfig;
};

#endif // STRATEGYLUOWENGANGSIMPLE_H
