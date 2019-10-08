#include "strategyjiaotansimple.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyJiaoTanSimple::StrategyJiaoTanSimple()
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    lastDiff_(0.0), currentDiff_(0.0),
    lastDiff1_(0.0), currentDiff1_(0.0),
    maClose10Tracker(10, closeVecPtr_),
    maClose17Tracker(17, closeVecPtr_),
    maClose200Tracker(200, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    backtestingConfig(BacktestingConfig::instance())
{

}

void StrategyJiaoTanSimple::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyJiaoTanSimple::onTick(Tick &tick)
{

}

void StrategyJiaoTanSimple::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    double maClose250 = maClose250Tracker.value();
    double maClose200 = maClose200Tracker.value();
    double maClose17 = maClose17Tracker.value();
    double maClose10 = maClose10Tracker.value();
    double diff = maClose17 - maClose200;
    lastDiff_ = currentDiff_;
    currentDiff_ = diff;
    double diff1 = maClose10 - maClose250;
    lastDiff1_ = currentDiff1_;
    currentDiff1_ = diff1;

    if (closeVecPtr_->size() <= 250) {
        return;
    }

    if (diff1 < 100.0 && close > maClose250 && diff > 0.0) {
        BPK(backtestingConfig->baseLot);
    }
    else if ( (lastDiff_ > 0.0 && currentDiff_ < 0.0) ||
              (lastDiff_ > 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ < 0.0)
            ) {
        SP(backtestingConfig->baseLot);
    }
    else if (close > maClose250 &&
             ((lastDiff1_ > 270.0 && currentDiff1_ < 270.0) ||
              (lastDiff1_ > 270.0 && currentDiff1_ == 270.0) ||
              (lastDiff1_ == 270.0 && currentDiff1_ < 270.0)
              )
             ) {
        SP(backtestingConfig->baseLot);
    }
    else if (diff1 > -100.0 && close < maClose250 && diff < 0.0) {
        SPK(backtestingConfig->baseLot);
    }
    else if ( (lastDiff_ < 0.0 && currentDiff_ > 0.0) ||
              (lastDiff_ < 0.0 && currentDiff_ == 0.0) ||
              (lastDiff_ == 0.0 && currentDiff_ > 0.0)
            ) {
        BP(backtestingConfig->baseLot);
    }
    else if (close < maClose250 &&
             ((lastDiff1_ < -270.0 && currentDiff1_ > -270.0) ||
              (lastDiff1_ < -270.0 && currentDiff1_ == -270.0) ||
              (lastDiff1_ == -270.0 && currentDiff1_ > -270.0)
              )
             ) {
        BP(backtestingConfig->baseLot);
    }
}

void StrategyJiaoTanSimple::openPosition(int volume)
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

void StrategyJiaoTanSimple::closePosition(int volume)
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

void StrategyJiaoTanSimple::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyJiaoTanSimple::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyJiaoTanSimple::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyJiaoTanSimple::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyJiaoTanSimple::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyJiaoTanSimple::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyJiaoTanSimple::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyJiaoTanSimple::getCloseMinute(KLineDataType &bar)
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

void StrategyJiaoTanSimple::onTrade(Trade &trade)
{

}

void StrategyJiaoTanSimple::onOrder(OrderRsp &order)
{

}

std::string StrategyJiaoTanSimple::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyJiaoTanSimple::instrumentType()
{
    return "i";
}
