#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

static int globalValue = 0;
std::mutex globalLock;
std::condition_variable globalConditionVariable;

using namespace std::literals::chrono_literals;

int main()
{
	int result = 0;
	bool ready = false;

	// reporter thread - needs to wait on worker thread to finish work
	std::thread reporter([&]() {
		std::unique_lock<std::mutex> lock(globalLock);
		if (!ready)
		{
			// blocks thread until condition variable is woken up
			// pass in the lock here to tell conditional variable which lock
			// to unlock as you wait for the results to be ready since
			// wait(std::mutex mut) atomically unlocked the mutex mut before
			// being blocked and going to sleep
			globalConditionVariable.wait(lock);
		}
		std::cout << "Reporter sees result:" << result << std::endl;
	});

	// worker thread
	std::thread worker([&]() {
		std::unique_lock<std::mutex> lock(globalLock);

		// artificially do work
		result = 42;
		ready = true;
		std::this_thread::sleep_for(3s);

		// notifies one other thread that is sleeping that mutex is free to access
		// and lock
		globalConditionVariable.notify_one();
	});

	reporter.join();
	worker.join();

	std::cout << "Main thread complete..." << std::endl;
}
