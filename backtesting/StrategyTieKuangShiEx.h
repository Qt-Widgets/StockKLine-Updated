#pragma once

#include <string>
#include <vector>
#include "Strategy.h"
#include "MA.h"

class StrategyTieKuangShiEx : public Strategy
{
public:
	StrategyTieKuangShiEx();
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

	MA maClose26Tracker;
	MA maClose10Tracker;
	MA maClose250Tracker;

	VecPtr captialVecPtr_;
	MA maCapital250Tracker_;

	int baselVolume_ = 1;
	int adjVolume_ = 0;

	double maxPosCross_ = 0.0;
	double minNegCross_ = 0.0;
};
