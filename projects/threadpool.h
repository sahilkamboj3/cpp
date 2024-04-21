#pragma once

#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <future>
#include <iostream>
#include <utility>
#include <type_traits>

class ThreadPool
{
public:
    ThreadPool(int numThreads)
    : numThreads_(numThreads)
    , quit_(false)
    {
        for (size_t i = 0; i < numThreads_; i++)
        {
            workers_.emplace_back([&]()
            {
                for (;;)
                {
                    using namespace std::chrono_literals;
                    
                    std::unique_lock<std::mutex> lock(mtx_);
                    cv_.wait(lock, [&]() { return !taskQueue_.empty() || quit_; });

                    if (quit_) break;
                    std::function<void()> task = taskQueue_.front();
                    taskQueue_.pop();
                    lock.unlock();
                    task();
                    std::this_thread::sleep_for(250ms);
                }
            });
        }
    }

    ~ThreadPool()
    {
        mtx_.lock();
        quit_ = true;
        mtx_.unlock();

        cv_.notify_all();

        for (size_t i = 0; i < numThreads_; i++)
        {
            workers_[i].join();
        }
    }

    template<typename F, typename ...Args>
    auto enqueue(F&& func, Args&& ...args) -> std::future<decltype(func(args...))>
    {
        using Task = std::packaged_task<decltype(func(args...))()>;

        auto funcBind = std::bind(std::forward<F>(func), std::forward<Args>(args)...);
        std::shared_ptr<Task> task = std::make_shared<Task>(funcBind);
        std::future<decltype(func(args...))> fut = task->get_future();
        auto task_wrapper = [task]() { (*task)(); };
        {
            std::lock_guard<std::mutex> lock(mtx_);
            taskQueue_.push(task_wrapper);
        }
        cv_.notify_one();
        return fut;
    }

private:
    size_t numThreads_;
    bool quit_;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> taskQueue_;
};