// Thread safe queue

#pragma once
#include "net_comon.h"
#include "net_message.h"

template <typename T>
class tsqueue
{
public:
	tsqueue() = default;
	// We don't allow tsqueue to be coppied because it's got mutexes in it
	tsqueue(const tsqueue<T>&) = delete;
	virtual ~tsqueue()
	{
		clear();
	}

public:
	const T& front()
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		return deq.front();
	}

	const T& back()
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		return deq.back();
	}

	void push_back(const T& item)
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		deq.push_back(item);
	}

	// Returns true if Queue has no items
	bool empty()
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		return deq.empty();
	}

	size_t size()
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		return deq.size();
	}

	// Clears Queue
	void clear()
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		deq.clear();
	}

	T pop_front()
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		auto t = std::move(deq.front());
		deq.pop_front();
		return t;
	}

	T pop_back()
	{
		std::lock_guard<std::mutex> guard(muxQueue);
		auto t = std::move(deq.back());
		deq.pop_back();
		return t;
	}

protected:
	std::mutex muxQueue;
	std::deque<T> deq;
};