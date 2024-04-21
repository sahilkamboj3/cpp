#include <iostream>
#include <vector>
#include <string>

#include "threadpool.h"

using namespace std::chrono_literals;

int add(int x, int y, int sleep_time)
{
    std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    int res = x + y;
    std::cout << "RES RUNNABLE THREAD:" << res << std::endl;
    return res;
}

std::string sadd(std::string x, std::string y, int sleep_time)
{
    std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
    std::string res = x + y;
    std::cout << "RES RUNNABLE THREAD:" << res << std::endl;
    return res;
}

int main()
{
    ThreadPool pool(5);
    std::vector<std::future<int>> futures;
    std::vector<std::future<std::string>> futuresS;

    futures.push_back(pool.enqueue(add, 1, 2, 5));
    futures.push_back(pool.enqueue(add, 3, 4, 0));
    futures.push_back(pool.enqueue(add, 5, 6, 3));
    futuresS.push_back(pool.enqueue(sadd, "base", "ball", 4));

    std::this_thread::sleep_for(5.5s);
    for (auto& fut : futures)
    {
        std::cout << "RES MAIN THREAD:" << fut.get() << std::endl;
    }
    for (auto& fut : futuresS)
    {
        std::cout << "RES MAIN THREAD:" << fut.get() << std::endl;
    }
}