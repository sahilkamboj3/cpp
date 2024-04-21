#include <iostream>
#include <mutex>
#include <vector>
#include <thread>

/*
Unless you are transferring ownership of a lock around,
you should use std::scoped_lock or std::lock_guard as opposed
to std::unique_lock (or if you required to lock and unlock)
in special cases for flexibility (ie. deferred locking or unlocked
after locking ie. conditional variables)

To avoid deadlocks, trying locking mutexes in the same order
to avoid them. You can also used std::scoped_lock or std::lock
to lock multiple mutexes. You can also look into hierarchical
locking of mutexes based on priority
*/

std::atomic_int ai = 0;
int i = 0;
std::mutex m, m1, m2;
constexpr unsigned int NUM_THREADS = 10000;

void func(int x) {
    std::lock_guard<std::mutex> guard(m);
    i += x;
}

void func2(int x) {
    std::lock(m1, m2); // lock both mutexes first
    // adopt_lock tells to inherit the lock, but don't try to
    // lock it again
    std::lock_guard<std::mutex> guard1(m1, std::adopt_lock);
    std::lock_guard<std::mutex> guard2(m2, std::adopt_lock);
    
    i += x;
}

void func3(int x) {
    ai += x;
}

void func4(int x) {
    // scoped_lock is similar to lock_guard, except you can lock
    // multiple mutexes at once
    std::scoped_lock guard(m, m1, m2);
    i += x;
}

void func5(int x) {
    // std::unique_lock is not copyable, it is only movable

    // defer_lock means to not lock in the constructor
    // We will have to lock ourselves later on via calling
    // lock on the unique_lock
    std::unique_lock<std::mutex> guard(m, std::defer_lock);
    guard.lock();
    i += x;
}

// std::shared_lock allows to share ownership of a mutex

// std::recursive_mutex allows to recursively lock a mutex
// - if using this, probably means you should change the design
//   of your project

int main() {
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.push_back(std::thread(func5, i));
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i].join();
    }

    std::cout << "i: " << i << std::endl;
}