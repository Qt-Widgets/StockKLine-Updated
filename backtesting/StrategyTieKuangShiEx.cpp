#include "StrategyTieKuangShiEx.h"
#include "Utility.h"
#include "BacktestingTradeGateway.h"

StrategyTieKuangShiEx::StrategyTieKuangShiEx()
    : closeVecPtr_(std::make_shared<std::vector<double>>()),
	captialVecPtr_(std::make_shared<std::vector<double>>()),
	lastDiff_(0.0), currentDiff_(0.0),
	maClose26Tracker(26, closeVecPtr_),
	maClose10Tracker(10, closeVecPtr_),
	maClose250Tracker(250, closeVecPtr_),
    maCapitalTracker_(250, captialVecPtr_),
    backtestingConfig(BacktestingConfig::instance())
{
    maCapitalTracker_.setPeriod(backtestingConfig->capitalPeriod);
}

void StrategyTieKuangShiEx::init(TradeGatewayPtr pTradeGateway)
{
	this->tradeGatewayPtr_ = pTradeGateway;
}

void StrategyTieKuangShiEx::onTick(Tick &tick)
{

}

void StrategyTieKuangShiEx::onBar(KLineDataType &bar)
{
	double close = bar.close_price;
	closeVecPtr_->push_back(close);
	double maClose250 = maClose250Tracker.value();
	double maClose26 = maClose26Tracker.value();
	double maClose10 = maClose10Tracker.value();
	int closeMinute = getCloseMinute(bar);
	double diff = maClose10 - maClose26;
	lastDiff_ = currentDiff_;
	currentDiff_ = diff;

	if (closeVecPtr_->size() <= 250) {
		return;
	}

    if (backtestingConfig->enableCapitalAjdustment) {

        double capital = std::static_pointer_cast<BacktestingTradeGateway>(tradeGatewayPtr_)->getAssetVaue();
        captialVecPtr_->push_back(capital);
        double maCapital250 = maCapitalTracker_.value();
        double capitalDiff = capital - maCapital250;
        double capitalDiffPercent = capitalDiff / maCapital250 * 100.0;

        // 资金曲线250均线采集满，才开始应用均线调整策略
        if (closeVecPtr_->size() > 250 + backtestingConfig->capitalPeriod) {

            if (capitalDiff <= 0.0) {
                maxPosCross_ = 0.0;
            }
            else {
                minNegCross_ = 0.0;
            }

            int volumeToDecrease = 0;
            if (maxPosCross_ < backtestingConfig->posThreshold1 && capitalDiffPercent > backtestingConfig->posThreshold1) {
                if (adjVolume_ >= backtestingConfig->posLotThreshold1) {
                    volumeToDecrease += backtestingConfig->posLotThreshold1;
                    adjVolume_ -= backtestingConfig->posLotThreshold1;
                    maxPosCross_ = backtestingConfig->posThreshold1 + 0.00000001;
                }
            }
            if (maxPosCross_ < backtestingConfig->posThreshold2 && capitalDiffPercent > backtestingConfig->posThreshold2) {
                if (adjVolume_ >= backtestingConfig->posLotThreshold2) {
                    volumeToDecrease += backtestingConfig->posLotThreshold2;
                    adjVolume_ -= backtestingConfig->posLotThreshold2;
                    maxPosCross_ = backtestingConfig->posThreshold2 + 0.00000001;
                }
            }
            if (maxPosCross_ < backtestingConfig->posThreshold3 && capitalDiffPercent > backtestingConfig->posThreshold3) {
                if (adjVolume_ >= backtestingConfig->posLotThreshold3) {
                    volumeToDecrease += backtestingConfig->posLotThreshold3;
                    adjVolume_ -= backtestingConfig->posLotThreshold3;
                    maxPosCross_ = backtestingConfig->posThreshold3 + 0.00000001;
                }
            }
            if (maxPosCross_ < backtestingConfig->posThreshold4 && capitalDiffPercent > backtestingConfig->posThreshold4) {
                if (adjVolume_ >= backtestingConfig->posLotThreshold4) {
                    volumeToDecrease += backtestingConfig->posLotThreshold4;
                    adjVolume_ -= backtestingConfig->posLotThreshold4;
                    maxPosCross_ = backtestingConfig->posThreshold4 + 0.00000001;
                }
            }
            if (maxPosCross_ < backtestingConfig->posThreshold5 && capitalDiffPercent > backtestingConfig->posThreshold5) {
                closeAllPosition();
                maxPosCross_ = backtestingConfig->posThreshold5 + 0.00000001;
            }
            if (volumeToDecrease > 0) {
                closePosition(volumeToDecrease);
            }

            // 清仓后，停止开仓，直到diff回到0轴
            if (maxPosCross_ >= backtestingConfig->posThreshold5 && capitalDiffPercent >= 0.0) {
                return;
            }

            int volumeToIncrease = 0;
            if (minNegCross_ > backtestingConfig->negThreshold1 && capitalDiffPercent < backtestingConfig->negThreshold1) {
                if (adjVolume_ + backtestingConfig->negLotThreshold1 <= backtestingConfig->totalAdjLot) {
                    volumeToIncrease += backtestingConfig->negLotThreshold1;
                    adjVolume_ += backtestingConfig->negLotThreshold1;
                    minNegCross_ = backtestingConfig->negThreshold1 - 0.00000001;
                }
            }
            if (minNegCross_ > backtestingConfig->negThreshold2 && capitalDiffPercent < backtestingConfig->negThreshold2) {
                if (adjVolume_ + backtestingConfig->negLotThreshold2 <= backtestingConfig->totalAdjLot) {
                    volumeToIncrease += backtestingConfig->negLotThreshold2;
                    adjVolume_ += backtestingConfig->negLotThreshold2;
                    minNegCross_ = backtestingConfig->negThreshold2 - 0.00000001;
                }
            }
            if (minNegCross_ > backtestingConfig->negThreshold3 && capitalDiffPercent < backtestingConfig->negThreshold3) {
                if (adjVolume_ + backtestingConfig->negLotThreshold3 <= backtestingConfig->totalAdjLot) {
                    volumeToIncrease += backtestingConfig->negLotThreshold3;
                    adjVolume_ += backtestingConfig->negLotThreshold3;
                    minNegCross_ = backtestingConfig->negThreshold3 - 0.00000001;
                }
            }
            openPosition(volumeToIncrease);
        }
    }

	if (close > maClose250 && diff > 0.0) {
        BPK(backtestingConfig->baseLot + adjVolume_);
	}
	else if (lastDiff_ > 0.0 && currentDiff_ < 0.0) {
        SP(backtestingConfig->baseLot + adjVolume_);
	}
	else if (close < maClose250 && diff < 0.0) {
        SPK(backtestingConfig->baseLot + adjVolume_);
	}
	else if (lastDiff_ < 0.0 && currentDiff_ > 0.0) {
        BP(backtestingConfig->baseLot + adjVolume_);
	}
}

void StrategyTieKuangShiEx::openPosition(int volume)
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

void StrategyTieKuangShiEx::closePosition(int volume)
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

void StrategyTieKuangShiEx::closeAllPosition()
{
	closePosition(std::numeric_limits<int>::max());
}

void StrategyTieKuangShiEx::BPK(int volume)
{
	tradeGatewayPtr_->closeShortAndOpenLong("instrumentID", volume);
}

void StrategyTieKuangShiEx::SPK(int volume)
{
	tradeGatewayPtr_->closeLongAndOpenShort("instrumentID", volume);
}

void StrategyTieKuangShiEx::SP(int volume)
{
	tradeGatewayPtr_->closeLong("instrumentID", volume);
}

void StrategyTieKuangShiEx::BP(int volume)
{
	tradeGatewayPtr_->closeShort("instrumentID", volume);
}

void StrategyTieKuangShiEx::SK(int volume)
{
	tradeGatewayPtr_->openShort("instrumentID", volume);
}

void StrategyTieKuangShiEx::BK(int volume)
{
	tradeGatewayPtr_->openLong("instrumentID", volume);
}

int StrategyTieKuangShiEx::getCloseMinute(KLineDataType &bar)
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

void StrategyTieKuangShiEx::onTrade(Trade &trade)
{

}

void StrategyTieKuangShiEx::onOrder(OrderRsp &order)
{

}

std::string StrategyTieKuangShiEx::name()
{
	return "TieKuangShi Extended Strategy";
}

std::string StrategyTieKuangShiEx::instrumentType()
{
	return "i";
}
