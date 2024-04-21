#pragma once

#include <cstddef>
#include <functional>
#include <iostream>

// heap-alocated MySharedPtr object
template<typename T>
class MySharedPtr
{
public:
    template<typename ...Args>
    MySharedPtr(Args... args)
    : users_(new size_t)
    {
        ptr_ = new T(std::forward<Args>(args)...);
        *users_ = 1;
    }

    MySharedPtr(MySharedPtr<T>& other)
    {
        copy(other);
    }

    MySharedPtr(MySharedPtr<T>&& other)
    {
        copy(other);
    }

    ~MySharedPtr()
    {
        (*users_)--;

        // no more users of object left
        if ((*users_) == 0)
        {
            std::cout << "RELEASING MEMORY" << std::endl;
            delete ptr_;
            delete users_;
        }
    }

    MySharedPtr& operator=(MySharedPtr& other)
    {
        copy(other);
        return *this;
    }

    T& operator*() const
    {
        return *ptr_;
    }

    void copy(MySharedPtr<T>& other)
    {
        ptr_ = other.ptr_;
        users_ = other.users_;

        (*users_)++;
    }

    size_t getCount() { return *users_; }

private:
    T* ptr_;
    size_t* users_;
};