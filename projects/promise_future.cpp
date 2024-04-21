#include <future>
#include <thread>
#include <iostream>

using namespace std::chrono_literals;

int main()
{
    std::promise<int> p;
    
    std::thread t([&p]() {
        std::future<int> fut = p.get_future();
        std::cout << "Waiting for res..." << std::endl;
        int res = fut.get();
        std::cout << "Got response " << res << std::endl;
    });
    
    std::this_thread::sleep_for(1s);
    p.set_value(5);
    t.join();
}