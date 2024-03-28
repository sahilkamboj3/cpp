#include <iostream>
#include <fstream>
#include <string>

/*
 * 
 * Default constructors are only generated if there are no user defined constructors present in the class.
 * 
 * Copy construction and copy assignment - 
 *  Generated independent of each other. If code requires copy constructor, but not provided, will be default generated.
 *  Similarly, if copy assignment is required but no explicitly declared, will also be default generated.
 *
 * Move construction and move assigned -
 *  Generated dependent on each other. If one is created, the other will not be generated. The reasoning is that
 *  if you declared one to be generated, that likely means the other needs to be declared for a similar
 *  reason, so you can't just generate one.
 * 
 * If the copy operations are defined, the move operations are not defined because if the copy operations
 * are defined, that likely means the generated move operations are not correct, so they won't be generated. (they are deleted)
 * The same goes for the other direction as well, if the move operations are defined, the copy
 * operations are not generated, they are deleted.
 * 
 * If a user declared destructor is created, the move operations are not generated because those
 * are expected to do special resource allocations since the destructor seems to be explicitly defined
 * for resource releasing. (they are deleted) The copy operations are default generated, but deprecated
 * meaning they should not be used because they could lead to undefined behavior. The reason they are
 * not deleted is because it would break too much code from C++98.
 * 
 * If the move operations are not declared, it will use the copy operations.
 * 
*/

class Bar {
public:
    Bar(const std::string s) { s_ = s; }
    void clean() { std::cout << "Bar cleaning up..." << std::endl; }

private:
    std::string s_;
};

class Foo
{
public:
    Foo() : bar_("s") { std::cout << "in default..." << std::endl; }
    Foo(Foo&&) : bar_("s") { std::cout << "in move..." << std::endl; }
    ~Foo() {
        bar_.clean();
    };

private:
    Bar bar_;
};

void func(Foo&&) { }

int main()
{
    Foo foo;
    std::cout << std::endl;
    Foo foo3(foo);
    Foo foo2(std::move(foo));
    std::cout << std::endl;
}
