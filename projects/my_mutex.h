#pragma once

#include <queue>
#include <stdexcept>
#include <thread>

using namespace std::chrono_literals;

class Mutex
{
public:
    Mutex()
    : locked_(false)
    {
    }
    Mutex(Mutex&) = delete;
    Mutex(Mutex&&) = delete;

    void lock()
    {
        queue_.push(std::this_thread::get_id());
        while (locked_ || queue_.front() != std::this_thread::get_id())
        {
            // std::this_thread::sleep_for(1ms);
        }
        locked_ = true;
        queue_.pop();
    }

    void unlock()
    {
        if (!locked_)
        {
            throw std::runtime_error("Mutex was not locked!");
        }
        locked_ = false;
    }

    bool getLocked() { return locked_; }

private:
    bool locked_;
    std::queue<std::thread::id> queue_;
};