#include "my_mutex.h"

#include <iostream>
#include <vector>
#include <thread>
#include <stdlib.h>

#define NUM_THREADS 100
#define DEBUG 0

using namespace std::chrono_literals;

Mutex mtx;
int glob = 0;

void foo(int id)
{
    int val = rand();

#if DEBUG
    std::cout << "foo_" << id << "() sleeping for " << (val % 10) << "ms" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(val % 100));
#endif

    mtx.lock();
    glob += 1;

#if DEBUG
    std::cout << "Have lock in foo_" << id << "()..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(val % 50));
#endif

    mtx.unlock();

#if DEBUG    
    std::cout << "Released lock in foo_" << id << "()..." << std::endl;
#endif
}

int main()
{
    std::vector<std::thread> threads;
    mtx.lock();

#if DEBUG
    std::cout << "Have lock in main()..." << std::endl;
#endif

    for (size_t i = 1; i <= NUM_THREADS; i++)
    {
        threads.emplace_back(foo, i);
    }

#if DEBUG
    std::this_thread::sleep_for(1s);
#endif

    mtx.unlock();

#if DEBUG
    std::cout << "Released lock in main()..." << std::endl;
#endif

    for (size_t i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }

    std::cout << std::boolalpha << "GLOB:" << glob << " " << (glob == NUM_THREADS) << std::endl;
}