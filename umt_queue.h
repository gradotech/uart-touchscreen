#ifndef __UMT_QUEUE_H__
#define __UMT_QUEUE_H__

#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class UMTQueue
{
public:
	void enqueue(T value);
	bool dequeue(T& value);

	void setBlocking(bool blocking);

private:
	std::queue<T> queue;
	mutable std::mutex mutex;
	std::condition_variable condition;
	bool blocking;
};

#include "umt_queue.cpp"

#endif /* __UMT_QUEUE_H__ */
