#include <thread>
#include "EventEngine.h"

void EventEngine::loop()
{
	while (running_.load()) {
		EventUniquePtr event = std::move(events_.take());
		switch ((*event).type) {
			case EventType::BarEvent:
				for (auto& handler : barHandlers_[(*event).data.kline.instrumentID]) {
					handler(*event);
				}
				break;
			case EventType::TickEvent:
				for (auto& handler : tickHandlers_[(*event).data.tick.instrumentID]) {
					handler(*event);
				}
				break;
			case EventType::TimerEvent:
				for (auto& handler : timerHandlers_) {
					handler(*event);
				}
				break;
		}
	}
}

void EventEngine::start() {
	thread_ = std::make_unique<std::thread>(&EventEngine::loop, this);
}

void EventEngine::join() {
	thread_->join();
}

void EventEngine::stop() 
{
	running_ = false;
}

void EventEngine::publish(EventUniquePtr event)
{
	events_.put(event);
}

// not thread-safe, so needs to be called before the event loop is started
void EventEngine::registerHandler(EventType type, std::vector<std::string> instrumentIDs, EventHandleFunc handler)
{
	for (auto& id : instrumentIDs) {
		switch (type) {
			case EventType::BarEvent:
				barHandlers_[id].push_back(handler);
				break;
			case EventType::TickEvent:
				tickHandlers_[id].push_back(handler);
				break;
			case EventType::TimerEvent:
				timerHandlers_.push_back(handler);
				break;
		}
	}
}
