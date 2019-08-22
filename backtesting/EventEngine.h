#ifndef EVENTENGINE_H
#define EVENTENGINE_H

#include <functional>
#include <vector>
#include <atomic>
#include <unordered_map>
#include "Event.h"
#include "BlockingQueue.h"

using EventHandleFunc = std::function<void(Event&)>;
using EventHandleFuncList = std::vector<EventHandleFunc>;

class EventEngine
{
public:
	EventEngine() : running_(true) {}
	void start();
	void stop();
	void join();
	void publish(EventUniquePtr event);
	void registerHandler(EventType type, std::vector<std::string> instrumentIDs, EventHandleFunc handler);

private:
	void loop();

private:
	BlockingQueue<EventUniquePtr> events_;
	std::unordered_map<std::string, EventHandleFuncList> barHandlers_;
	std::unordered_map<std::string, EventHandleFuncList> tickHandlers_;
	EventHandleFuncList timerHandlers_;

	std::atomic<bool> running_;
	std::unique_ptr<std::thread> thread_;
};

using EventEnginePtr = std::shared_ptr<EventEngine>;

#endif
