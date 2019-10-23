#include "strategyluowengangsimpleex.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyLuoWenGangSimpleEx::StrategyLuoWenGangSimpleEx(TradeGatewayPtr pTradeGatewayforSimple)
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
    captialVecPtr_(std::make_shared<std::vector<double>>()),
    simpleStrategyCaptialVecPtr_(std::make_shared<std::vector<double>>()),
    lastDiff_(0.0), currentDiff_(0.0),
    lastMA12_(0.0), currentMA12_(0.0),
    lastMA250_(0.0), currentMA250_(0.0),
    maClose5Tracker(5, closeVecPtr_),
    maClose12Tracker(12, closeVecPtr_),
    maClose250Tracker(250, closeVecPtr_),
    maCapitalTracker_(250, captialVecPtr_),
    maSimpleStrategyCapital250Tracker_(250, simpleStrategyCaptialVecPtr_),
    tradeGatewayForSimplePtr_(pTradeGatewayforSimple),
    backtestingConfig(BacktestingConfig::instance())
{
    maCapitalTracker_.setPeriod(backtestingConfig->capitalPeriod);
}

void StrategyLuoWenGangSimpleEx::init(TradeGatewayPtr pTradeGateway)
{
    this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyLuoWenGangSimpleEx::onTick(Tick &tick)
{

}

int StrategyLuoWenGangSimpleEx::min(int a, int b)
{
    return a < b ? a : b;
}

void StrategyLuoWenGangSimpleEx::onBar(KLineDataType &bar)
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

    if (backtestingConfig->enableCapitalAjdustment) {

        auto gateWay = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_);
        // clear the signal each time
        gateWay->recordAdjustmentSignal("");

        // start calculate the MA capital line for the simple strategy
        double simpleStrategyCapital = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayForSimplePtr_)->getAssetVaue();
        simpleStrategyCaptialVecPtr_->push_back(simpleStrategyCapital);
        double maSimpleStrategyCapital250 = maSimpleStrategyCapital250Tracker_.value();
        maxSimpleStrategyCapital_ = std::fmax(maxSimpleStrategyCapital_, simpleStrategyCapital);

        // note: the benchmark is all about the simple strategy. it has nothing to do with the adjustment based strategy.
        double capitalDiff = simpleStrategyCapital - maSimpleStrategyCapital250;
        double backtrack = simpleStrategyCapital - maxSimpleStrategyCapital_;

        // 资金曲线250均线采集满，才开始应用均线调整策略
        if (closeVecPtr_->size() > 250 + backtestingConfig->capitalPeriod) {

            if (capitalDiff <= 0.0) {
                maxPosCross_ = 0.0;
            }
            if (backtrack >= 0.0) {
                addLot2Trigerred = false;
                addLot3Trigerred = false;
            }

            if (maxPosCross_ < backtestingConfig->decLotDiffThreshold2 && capitalDiff > backtestingConfig->decLotDiffThreshold2) {
                closeAllPosition();
                maxPosCross_ = backtestingConfig->decLotDiffThreshold2 + 0.00000001;
                gateWay->recordAdjustmentSignal("Q");
            }
            else if (maxPosCross_ < backtestingConfig->decLotDiffThreshold1 && capitalDiff > backtestingConfig->decLotDiffThreshold1) {
                int volumeToCut = min(adjVolume_, backtestingConfig->baseLot);
                adjVolume_ -= volumeToCut;
                maxPosCross_ = backtestingConfig->decLotDiffThreshold1 + 0.00000001;
                gateWay->recordAdjustmentSignal("J");
            }

            // 清仓后，停止开仓，直到diff回到0轴
            if (maxPosCross_ >= backtestingConfig->decLotDiffThreshold2 && capitalDiff >= 0.0) {
                return;
            }

            if (!addLot3Trigerred &&
                    capitalDiff < backtestingConfig->addLotDiffThreshold1 &&
                    backtrack < backtestingConfig->addLotBacktrackThreshold2) {
                adjVolume_ = 3 * backtestingConfig->baseLot;
                gateWay->recordAdjustmentSignal("3");
                addLot3Trigerred = true;
            }
            else if (!addLot2Trigerred && !addLot3Trigerred &&
                     ((capitalDiff < backtestingConfig->addLotDiffThreshold1 && backtrack < backtestingConfig->addLotBacktrackThreshold1) ||
                      (backtrack < backtestingConfig->addLotBacktrackThreshold2)
                     )
               ) {
                adjVolume_ = 2 * backtestingConfig->baseLot;
                gateWay->recordAdjustmentSignal("2");
                addLot2Trigerred = true;
            }
        }
    }

    if ( (lastMA12_ < lastMA250_ && currentMA12_ > currentMA250_) ||
          (lastMA12_ < lastMA250_ && currentMA12_ == currentMA250_) ||
          (lastMA12_ == lastMA250_ && currentMA12_ > currentMA250_)
          ) {
        BPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastMA12_ > lastMA250_ && currentMA12_ < currentMA250_) ||
              (lastMA12_ > lastMA250_ && currentMA12_ == currentMA250_) ||
              (lastMA12_ == lastMA250_ && currentMA12_ < currentMA250_)
              ) {
        SPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ < -180.0 && currentDiff_ > -180.0) ||
              (lastDiff_ < -180.0 && currentDiff_ == -180.0) ||
              (lastDiff_ == -180.0 && currentDiff_ > -180.0)
            ) {
        BPK(backtestingConfig->baseLot + adjVolume_);
    }
    else if ( (lastDiff_ > 180.0 && currentDiff_ < 180.0) ||
              (lastDiff_ > 180.0 && currentDiff_ == 180.0) ||
              (lastDiff_ == 180.0 && currentDiff_ < 180.0)
            ) {
        SPK(backtestingConfig->baseLot + adjVolume_);
    }
}

void StrategyLuoWenGangSimpleEx::openPosition(int volume)
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

void StrategyLuoWenGangSimpleEx::closePosition(int volume)
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

void StrategyLuoWenGangSimpleEx::closeAllPosition()
{
    closePosition(std::numeric_limits<int>::max());
}

void StrategyLuoWenGangSimpleEx::BPK(int volume)
{
    tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyLuoWenGangSimpleEx::SPK(int volume)
{
    tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyLuoWenGangSimpleEx::SP(int volume)
{
    tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyLuoWenGangSimpleEx::BP(int volume)
{
    tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyLuoWenGangSimpleEx::SK(int volume)
{
    tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyLuoWenGangSimpleEx::BK(int volume)
{
    tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyLuoWenGangSimpleEx::getCloseMinute(KLineDataType &bar)
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

void StrategyLuoWenGangSimpleEx::onTrade(Trade &trade)
{

}

void StrategyLuoWenGangSimpleEx::onOrder(OrderRsp &order)
{

}

std::string StrategyLuoWenGangSimpleEx::name()
{
    return "TieKuangShi Extended Strategy";
}

std::string StrategyLuoWenGangSimpleEx::instrumentType()
{
    return "i";
}
