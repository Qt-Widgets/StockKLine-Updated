#include "strategyluowengangsimple.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyLuoWenGangSimple::StrategyLuoWenGangSimple()
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    lastDiff_(0.0), currentDiff_(0.0),
    lastMA12_(0.0), currentMA12_(0.0),
    lastMA250_(0.0), currentMA250_(0.0),
    maClose5Tracker(5, closeVecPtr_),
    maClose12Tracker(12, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    backtestingConfig(BacktestingConfig::instance())
{

}

void StrategyLuoWenGangSimple::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyLuoWenGangSimple::onTick(Tick &tick)
{

}

void StrategyLuoWenGangSimple::onBar(KLineDataType &bar)
{
    double close = bar.close_price;
    closeVecPtr_->push_back(close);
    double maClose250 = maClose250Tracker.value();
    double maClose12 = maClose12Tracker.value();
    double maClose5 = maClose5Tracker.value();
    double diff = maClose5 - maClose250;
    lastDiff_ = currentDiff_;
    currentDiff_ = diff;
    lastMA12_ = currentMA12_;
    currentMA12_ = maClose12;
    lastMA250_ = currentMA250_;
    currentMA250_ = maClose250;

    if (closeVecPtr_->size() <= 250) {
        return;
    }

    if ( (lastMA12_ < lastMA250_ && currentMA12_ > currentMA250_) ||
          (lastMA12_ < lastMA250_ && currentMA12_ == currentMA250_) ||
          (lastMA12_ == lastMA250_ && currentMA12_ > currentMA250_)
          ) {
        BPK(backtestingConfig->baseLot);
    }
    else if ( (lastMA12_ > lastMA250_ && currentMA12_ < currentMA250_) ||
              (lastMA12_ > lastMA250_ && currentMA12_ == currentMA250_) ||
              (lastMA12_ == lastMA250_ && currentMA12_ < currentMA250_)
              ) {
        SPK(backtestingConfig->baseLot);
    }
    else if ( (lastDiff_ < -180.0 && currentDiff_ > -180.0) ||
              (lastDiff_ < -180.0 && currentDiff_ == -180.0) ||
              (lastDiff_ == -180.0 && currentDiff_ > -180.0)
            ) {
        BPK(backtestingConfig->baseLot);
    }
    else if ( (lastDiff_ > 180.0 && currentDiff_ < 180.0) ||
              (lastDiff_ > 180.0 && currentDiff_ == 180.0) ||
              (lastDiff_ == 180.0 && currentDiff_ < 180.0)
            ) {
        SPK(backtestingConfig->baseLot);
    }
}

void StrategyLuoWenGangSimple::openPosition(int volume)
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

void StrategyLuoWenGangSimple::closePosition(int volume)
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

void StrategyLuoWenGangSimple::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyLuoWenGangSimple::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyLuoWenGangSimple::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyLuoWenGangSimple::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyLuoWenGangSimple::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyLuoWenGangSimple::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyLuoWenGangSimple::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyLuoWenGangSimple::getCloseMinute(KLineDataType &bar)
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

void StrategyLuoWenGangSimple::onTrade(Trade &trade)
{

}

void StrategyLuoWenGangSimple::onOrder(OrderRsp &order)
{

}

std::string StrategyLuoWenGangSimple::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyLuoWenGangSimple::instrumentType()
{
    return "i";
}
