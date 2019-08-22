#ifndef STRATEEGY_H
#define STRATEEGY_H

#include "KlineData.h"
#include "TradeGateway.h"

class Strategy
{
public:
	virtual void init(TradeGatewayPtr pTradeGateway) = 0;
	virtual void onTick(Tick &tick) = 0;
	virtual void onBar(KLineDataType &bar) = 0;
	virtual void onTrade(Trade &trade) = 0;
	virtual void onOrder(OrderRsp &order) = 0;
	virtual std::string name() = 0;
	virtual std::string instrumentType() = 0;
};

using StrategyPtr = std::shared_ptr<Strategy>;

#endif
