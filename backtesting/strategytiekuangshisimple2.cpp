#include "strategytiekuangshisimple2.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyTieKuangShiSimple2::StrategyTieKuangShiSimple2()
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    lastD_(0.0), currentD_(0.0), lastK_(0.0), currentK_(0.0),
    maClose5Tracker(5, closeVecPtr_),
    maClose6Tracker(6, closeVecPtr_),
    maClose20Tracker(20, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    backtestingConfig(BacktestingConfig::instance()),
    windowedMaxMin_(60), lastRsv_(0.0)
{

}

void StrategyTieKuangShiSimple2::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyTieKuangShiSimple2::onTick(Tick &tick)
{

}
/*
    SMA(X,N,M) 求X的N个周期内的移动平均。M为权重。
    计算公式：SMA(N)=SMA(N-1)*(N-M)/N+X(N)*M/N
 */
void StrategyTieKuangShiSimple2::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    windowedMaxMin_.update(close);
    double maClose250 = maClose250Tracker.value();
    double maClose5 = maClose5Tracker.value();
    double maClose6 = maClose6Tracker.value();
    double maClose20 = maClose20Tracker.value();
    double diff = maClose6 - maClose20;
    double diff1 = maClose5 - maClose250;
    lastD_ = currentD_;
    lastK_ = currentK_;
    double rsv = (close - windowedMaxMin_.getMin()) / (windowedMaxMin_.getMax() - windowedMaxMin_.getMin()) * 100.0;
    currentK_ = lastRsv_ * (20 - 1) / 20 + rsv * 1 / 20;
    currentD_ = lastK_ * (60 - 1) / 60 + currentK_ * 1 / 60;
    lastRsv_ = rsv;

    if (closeVecPtr_->size() <= 250) {
        return;
    }

    if (diff1 < 42 && close > maClose250 && diff > 0.0) {
        BPK(backtestingConfig->baseLot);
    }
    else if (diff1 > -42 && close < maClose250 && diff < 0.0) {
        SPK(backtestingConfig->baseLot);
    }
    else if (close > maClose250 && crossDK()) {
        SP(backtestingConfig->baseLot);
    }
    else if (diff1 > 42 && close > maClose250 && crossKD()) {
        BK(backtestingConfig->baseLot);
    }
    else if (close < maClose250 && crossKD()) {
        BP(backtestingConfig->baseLot);
    }
    else if (diff1 < -42 && close < maClose250 && crossDK()) {
        SK(backtestingConfig->baseLot);
    }
}

bool StrategyTieKuangShiSimple2::crossDK()
{
    return (lastD_ < lastK_ && currentD_ > currentK_) ||
            (lastD_ == lastK_ && currentD_ > currentK_) ||
            (lastD_ < lastK_ && currentD_ == currentK_);
}

bool StrategyTieKuangShiSimple2::crossKD()
{
    return (lastK_ < lastD_ && currentK_ > currentD_) ||
            (lastK_ == lastD_ && currentK_ > currentD_) ||
            (lastK_ < lastD_ && currentK_ == currentD_);
}

void StrategyTieKuangShiSimple2::openPosition(int volume)
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

void StrategyTieKuangShiSimple2::closePosition(int volume)
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

void StrategyTieKuangShiSimple2::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyTieKuangShiSimple2::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyTieKuangShiSimple2::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyTieKuangShiSimple2::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyTieKuangShiSimple2::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyTieKuangShiSimple2::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyTieKuangShiSimple2::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyTieKuangShiSimple2::getCloseMinute(KLineDataType &bar)
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

void StrategyTieKuangShiSimple2::onTrade(Trade &trade)
{

}

void StrategyTieKuangShiSimple2::onOrder(OrderRsp &order)
{

}

std::string StrategyTieKuangShiSimple2::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyTieKuangShiSimple2::instrumentType()
{
    return "i";
}
