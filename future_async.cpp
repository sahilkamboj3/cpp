/*
std::async - means without synchronization, can launch a background thread and periodically
query for the status of the future(result) returns by this function running in
the separate thread. When the result is ready from the background thread, it can
then by used by the caller thread
*/

#include <iostream>
#include <chrono>
#include <future>

using namespace std::literals::chrono_literals;

bool fetchData()
{
	const int CHUNKS = 5;
	for (int i = 0; i < CHUNKS; i++)
	{
		std::this_thread::sleep_for(250ms);
		std::cout << "chunk:" << i << " has been fetched" << std::endl;
	}

	std::cout << "all chunks have been fetched..." << std::endl;
	return true;
}

int main()
{
	// this is launching the function to run a separate background thread 
	// asynchronously so we can then wait for the result sometime in the future
	std::future<bool> fetchDataFuture = std::async(std::launch::async, &fetchData);

	while (true)
	{
		std::cout << "inside infinite while loop..." << std::endl;

		// this is waiting for 1ms (timeout) for the future results to be ready
		// if it is not ready, this thread will move on to not stay blocked and
		// through this infinite while loop, will eventually query for the future
		// status again
		std::future_status status = fetchDataFuture.wait_for(std::chrono::milliseconds(1));
		if (status == std::future_status::ready)
		{
			std::cout << "Data has been returned to main thread..." << std::endl;
			break;
		}

		// Other status option is deferred to run the function lazily when the
		// get() function is called on the future to return the results

		std::this_thread::sleep_for(50ms);
	}

	std::cout << "Program ended..." << std::endl;
}
