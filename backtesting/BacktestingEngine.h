#pragma once

#include "BacktestingTradeGateway.h"
#include "EventEngine.h"

class BacktestingEngine
{
public:
	BacktestingEngine(std::shared_ptr<EventEngine> eventEngine);
	void start();

	BacktestingEngine() = delete;
	BacktestingEngine(const BacktestingEngine&) = delete;
	BacktestingEngine& operator=(const BacktestingEngine&) = delete;

private:
	std::unique_ptr<BacktestingTradeGateway> btGateway_;
	std::shared_ptr<EventEngine> eventEngine_;
};
