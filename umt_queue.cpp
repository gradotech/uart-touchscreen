#include "umt_queue.h"

template <typename T>
void UMTQueue<T>::enqueue(T value)
{
	std::lock_guard<std::mutex> lock(mutex);

	queue.push(std::move(value));

	if (blocking)
		condition.notify_one();
}

template <typename T>
bool UMTQueue<T>::dequeue(T& value)
{
	std::unique_lock<std::mutex> lock(mutex);

	if (blocking) {
		condition.wait(lock, [this] {
			return !queue.empty();
		});
	}

	if (queue.empty())
		return false;

	value = std::move(queue.front());
	queue.pop();

	return true;
}

template <typename T>
void UMTQueue<T>::setBlocking(bool blocking)
{
	this->blocking = blocking;
}
