/*
The atomic library in C++ allows to atomically modify certain data types that
are supported in an atomic fashion with a wrapper without having to explicit
add mutexes or lock_guards or really unneeded bloat. This is a simple way
to modify supported data types atomically via overriden functions/operations.
*/

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

// ie. std::atomic_int alias for std::atomic<int>
// Others include std::atomic_bool, std::atomic_char, etc
static std::atomic_int glob = 0;

void foo()
{
	// Must be ++ since operator++() overridden for atomic_int to support
	// increments atomically. Using 'glob = glob + 1' would still have the
	// data race issue since that is not overridden in std::atomic_int
	glob++; 
}

int main()
{
	const int NUM_THREADS = 5000;
	
	std::vector<std::thread> threads;
	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads.emplace_back(&foo);
	}

	for (int i = 0; i < NUM_THREADS; i++)
	{
		threads[i].join();
	}

	std::cout << "glob:" << glob << std::endl;
}

