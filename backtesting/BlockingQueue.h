#ifndef CONCURRENT_QUEUE_
#define CONCURRENT_QUEUE_

#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>

template <typename T>
class BlockingQueue
{
public:

	T take()
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		while (queue_.empty())
		{
			cond_.wait(mlock);
		}
		T val = std::move(queue_.front());
		queue_.pop_front();
		return std::move(val);
	}

	void put(T& item)
	{
		std::unique_lock<std::mutex> mlock(mutex_);
		queue_.push_back(std::move(item));
		mlock.unlock();
		cond_.notify_one();
	}
	BlockingQueue() = default;
	BlockingQueue(const BlockingQueue&) = delete;            // disable copying
	BlockingQueue& operator=(const BlockingQueue&) = delete; // disable assignment

private:
	std::deque<T> queue_;
	std::mutex mutex_;
	std::condition_variable cond_;
};

#endif
