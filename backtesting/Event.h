#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include "KlineData.h"

enum class EventType { BarEvent, TickEvent, TimerEvent };

struct Event
{
	EventType type;
	union {
		KLineDataType kline;
		Tick tick;
		int timerPeriod;
	} data;
};

using EventUniquePtr = std::unique_ptr<Event>;
using EventSharedPtr = std::shared_ptr<Event>;

#endif
