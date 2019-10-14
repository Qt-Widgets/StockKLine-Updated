#include "StrategyTieKuangShiSimple.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyTieKuangShiSimple::StrategyTieKuangShiSimple()
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    lastDiff_(0.0), currentDiff_(0.0),
    maClose26Tracker(26, closeVecPtr_),
    maClose10Tracker(10, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    maClose5Tracker(5, closeVecPtr_),
    backtestingConfig(BacktestingConfig::instance())
{

}

void StrategyTieKuangShiSimple::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyTieKuangShiSimple::onTick(Tick &tick)
{

}

void StrategyTieKuangShiSimple::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    double maClose250 = maClose250Tracker.value();
    double maClose26 = maClose26Tracker.value();
    double maClose10 = maClose10Tracker.value();
    double maClose5 = maClose5Tracker.value();
    int closeMinute = getCloseMinute(bar);
    double diff = maClose10 - maClose26;
    lastDiff_ = currentDiff_;
    currentDiff_ = diff;
    double diff1 = maClose5 - maClose250;

    if (closeVecPtr_->size() <= 250) {
        return;
    }

    if (diff1 < backtestingConfig->tieKuangShiN && close > maClose250 && diff > 0.0) {
        BPK(backtestingConfig->baseLot);
    }
    else if ( (lastDiff_ > 0.0 && currentDiff_ < 0.0) ||
              (lastDiff_ > 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ < 0.0)
            ) {
        SP(backtestingConfig->baseLot);
    }
    else if (diff1 > -1.0 * backtestingConfig->tieKuangShiN && close < maClose250 && diff < 0.0) {
        SPK(backtestingConfig->baseLot);
    }
    else if ( (lastDiff_ < 0.0 && currentDiff_ > 0.0) ||
              (lastDiff_ < 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ > 0.0)
            ) {
        BP(backtestingConfig->baseLot);
    }
}

void StrategyTieKuangShiSimple::openPosition(int volume)
{
    auto gateWay = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_);
    int longTradeCount = gateWay->getLongTradeCount();
    int shortTradeCount = gateWay->getShortTradeCount();
    if (longTradeCount > 0) {
        BK(volume);
    }
    else if (shortTradeCount > 0) {
        SK(volume);
    }
}

void StrategyTieKuangShiSimple::closePosition(int volume)
{
    auto gateWay = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_);
    int longTradeCount = gateWay->getLongTradeCount();
    int shortTradeCount = gateWay->getShortTradeCount();
    if (longTradeCount > 0) {
        if (volume > longTradeCount) {
            SP(longTradeCount);
        }
        else {
            SP(volume);
        }
    }
    else if (shortTradeCount > 0) {
        if (volume > shortTradeCount) {
            BP(shortTradeCount);
        }
        else {
            BP(volume);
        }
    }
}

void StrategyTieKuangShiSimple::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyTieKuangShiSimple::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyTieKuangShiSimple::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyTieKuangShiSimple::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyTieKuangShiSimple::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyTieKuangShiSimple::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyTieKuangShiSimple::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyTieKuangShiSimple::getCloseMinute(KLineDataType &bar)
{
    int minutesOfDay = getMinutesOfDay(bar.update_time);

    int minutesToClose = (10 * 60 + 15) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    minutesToClose = (11 * 60 + 30) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    minutesToClose = (15 * 60) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    minutesToClose = (23 * 60 + 30) - minutesOfDay;
    if (minutesToClose > 0) return minutesToClose;

    return minutesToClose;
}

void StrategyTieKuangShiSimple::onTrade(Trade &trade)
{

}

void StrategyTieKuangShiSimple::onOrder(OrderRsp &order)
{

}

std::string StrategyTieKuangShiSimple::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyTieKuangShiSimple::instrumentType()
{
    return "i";
}
