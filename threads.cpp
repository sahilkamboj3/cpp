/*
Allow to have multiple paths/tasks running at once in parallel.
Concurrency is when tasks can happen at the same time, but there is the chance
of having to wait on shared resources. Parallelism is when multiple tasks
can occur separately instantaneously (there is no dependency).

ie. concurrency example - multiple readers, 1 writer

mutex - mutual exclusion (only one thread has access to object). Threads can
eat up cycles constantly polling to see if the mutex is unlocked and available

deadlock - thread doesn't return lock or tell other threads the lock has been
released so other threads are stuck waiting since they don't know/can't lock
the mutex

lock_guard - mutex wrapper where when the destructor is called, any of the
held mutexes are going to be unlocked, even in the case of exceptions. This
is preferred over having naked mutexes since it is a source of bugs to
remember to unlock the mutex in the different flow paths of the function
*/

#include <unistd.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex globLock;
static int glob = 0;

void increment_glob_mutex()
{
	globLock.lock();
	glob++;
	globLock.unlock();
}

void increment_glob_lockguard()
{
	// lock_guard here ensures the globLock global mutex is going to be released
	// when the lock_guard destructor is called at the end of the function, even
	// if there is an exception thrown
	std::lock_guard<std::mutex> lockGuard(globLock);
	try
	{
		glob++;
		throw "Forced error thrown...";
	} catch(...)
	{
		std::cout << "Exception thrown..." << std::endl;
		return;
	}
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

	const int NUM_THREADS = 100;
	std::vector<std::thread> threads;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		// threads.push_back(std::thread(&increment_glob_mutex));
		threads.push_back(std::thread(&increment_glob_lockguard));
	}

	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads[i].join();
	}

	std::cout << "glob:" << glob << std::endl;
}
