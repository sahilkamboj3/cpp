#include "my_shared_ptr.h"

#include <iostream>

void foo(MySharedPtr<int> ptr)
{
    std::cout << "foo::ptr::USERS:" << ptr.getCount() << std::endl;
}

int main()
{
    MySharedPtr<int> obj(5);
    std::cout << "USERS:" << obj.getCount() << std::endl;

    {
        MySharedPtr<int> newobj(obj);
        std::cout << "USERS:" << obj.getCount() << std::endl;
        std::cout << "USERS:" << newobj.getCount() << std::endl;

        MySharedPtr<int> newobjmoved(std::move(obj));
        std::cout << "USERS:" << newobj.getCount() << std::endl;
    }

    std::cout << "USERS:" << obj.getCount() << std::endl;

    MySharedPtr<int> newobj = obj;
    std::cout << "USERS:" << obj.getCount() << std::endl;
    std::cout << "USERS:" << newobj.getCount() << std::endl;

    foo(newobj);

    std::cout << "USERS:" << obj.getCount() << std::endl;
    std::cout << "USERS:" << newobj.getCount() << std::endl;

    std::cout << "OBJ VALUE:" << *obj << std::endl;
    std::cout << "NEWOBJ VALUE:" << *newobj << std::endl;

    int newValue = 10;
    *obj = newValue;
    std::cout << "SET VALUE TO " << newValue << std::endl;

    std::cout << "OBJ VALUE:" << *obj << std::endl;
    std::cout << "NEWOBJ VALUE:" << *newobj << std::endl;
}