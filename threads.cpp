/*
Allow to have multiple paths/tasks running at once in parallel.
Concurrency is when tasks can happen at the same time, but there is the chance
of having to wait on shared resources. Parallelism is when multiple tasks
can occur separately instantaneously (there is no dependency).

ie. concurrency example - multiple readers, 1 writer

mutex - mutual exclusion (only one thread has access to object)

deadlock - thread doesn't return lock or tell other threads the lock has been
released so other threads are stuck waiting since they don't know/can't lock
the mutex
*/

#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex globLock;
static int glob = 0;

void increment_glob()
{
	globLock.lock();
	glob++;
	globLock.unlock();
}

void DoWork()
{
	using namespace std::literals::chrono_literals;

	int i = 0;
	while (i < 5)
	{
		std::cout << "DoWork()..." << std::endl;
		std::this_thread::sleep_for(1s); // or sleep(1);
		i++;
	}
}

void GetInput(int x)
{
	std::cin.get();
	std::cout << "GetInput(" << x << ")" << std::endl;
}

int main()
{
	std::thread io(&GetInput, 10);
	std::thread worker(&DoWork);
	std::thread lamdaThread([](int x) {
		std::cout << "LAMBDA:" << x << std::endl;
	}, 100);

 	// wait on this thread until the worker thread has completed.
	// when the worker and io thread finishes, then we resume on this thread
	io.join();
	worker.join();
	lamdaThread.join();

	const int NUM_THREADS = 1000;
	std::vector<std::thread> threads;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads.push_back(std::thread(&increment_glob));
	}

	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads[i].join();
	}

	std::cout << "glob:" << glob << std::endl;
}
