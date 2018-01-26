#pragma once

#include <mutex>
#include <condition_variable>

class semaphore 
{
public:
	semaphore(int value);
	~semaphore();
	void wait()
	{
		std::unique_lock<std::mutex> lock{ mutex };
		if (--count<0) 
		{ // count is not enough ?
			condition.wait(lock, [&]()->bool{ return wakeups>0; }); // suspend and wait ...
			--wakeups;  // ok, me wakeup !
		}
	}
	void signal()
	{
		std::lock_guard<std::mutex> lock{ mutex };
		if (++count <= 0) 
		{ // have some thread suspended ?
			++wakeups;
			condition.notify_one(); // notify one !
		}
	}
private:
	int count;//锁数
	int wakeups;//等待被唤醒的线程数
	std::mutex mutex;
	std::condition_variable condition;
};

extern semaphore sendSemaphore;
extern semaphore recvSemaphore;


